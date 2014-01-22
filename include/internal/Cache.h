#ifndef INTERNAL_CACHE_H
#define INTERNAL_CACHE_H

#include "macros.h"
#include "ForwardDeclarations.h"
#include "GPBase.h"
#include "MatrixBase.h"
#include "ScalarBase.h"
#include "CachePlugins.h"

#include <typeinfo>
#include <type_traits>
#include <memory>

namespace GP {
namespace internal {

namespace {
// define parent type of object to be cached
template <class Object>
using Parent = typename std::conditional
    <std::is_convertible<Object, MatrixBase<Object> >::value,
     MatrixBase<Cache<Object> >,
     typename std::conditional<
         std::is_convertible<Object, ScalarBase<Object> >::value,
         ScalarBase<Cache<Object> >,
         GPBase<Cache<Object> >
        >::type
    >::type;
//helper to strip const reference
template <class Object>
using StripConstRef = typename std::remove_const
    <typename std::remove_reference<Object>::type>::type;
//if EigenExpr is an Eigen expression, then this returns whatever the expression
//can be evaluated into. Can be T or const T& depending on expression.
template <class EigenExpr> struct EigenEval {
  typedef StripConstRef<EigenExpr> bare_eigen;
  typedef typename Eigen::internal::eval<bare_eigen>::type eigen_eval_type;
  typedef StripConstRef<eigen_eval_type> type;
  typedef std::true_type value_type;
};
template<class OtherExpr> struct OtherEval {
    typedef OtherExpr type;
  typedef std::false_type value_type;
};
//check if Object has an eval() member
//see http://stackoverflow.com/a/21246279/958110
//if it has, return a call to eval() as type
//if not, return the type itself
namespace {
  template <typename T>
  auto has_eval_impl(void*)
    -> decltype(std::declval<T>().eval(), EigenEval<T>());
  template <typename T> auto has_eval_impl(...)->decltype(OtherEval<T>());
}
template <typename T>
struct has_eval : decltype(has_eval_impl<T>(nullptr)) {};
}

// traits class. See below for remarks on result_type
template <class Object> struct traits<Cache<Object> > {
typedef typename traits<Object>::scalar_type scalar_type;
typedef typename has_eval<typename traits<Object>::result_type>::type
    result_type;
typedef typename has_eval<typename Object::result_type>::value_type eval_called;
};

//! Cache class
/* Template it on a GPBase object to cache its get() return value.
   \note The result_type can be different from Object::result_type
   If Object::result_type can be cast to an Eigen EvalReturnType, it will be
   cast to it. Otherwise, the result_type will be the same.
   The rationale is that you don't want to cache an expression template that
   produces a matrix, but you want to store the computed matrix. On the other
   hand LDLT decomposition objects etc. should be stored as-is.
   \note it is supposed to behave transparently most of the time. The idea was
   to follow the Compound object pattern.
*/
template <class Object>
class Cache : public Parent<Object>, public CachePlugins<Object> {
 public:
  typedef typename traits<Cache>::scalar_type scalar_type;
  typedef typename traits<Cache>::result_type result_type;
  typedef typename traits<Cache>::eval_called eval_called;

 private:
  class Data {
   private:
    mutable unsigned version_;
    mutable result_type val_;

   private:
    void set_value(const Object& obj, std::true_type) const {
      version_ = obj.get_version();
      val_ = obj.get().eval();  // force evaluation
    }
    void set_value(const Object& obj, std::false_type) const {
      version_ = obj.get_version();
      val_ = obj.get();  // obj.get() does not have eval(): store as-is
    }

   public:
    const result_type& get() const { return val_; }
    unsigned get_version() const { return version_; }
    void set_value(const Object& obj) {
      set_value(obj, eval_called());
    }

    Data(const Object& obj) {
      set_value(obj);
      LOG("   Cache data init : version = " << version_ << std::endl);
    }
  };

  const Object& obj_;
  mutable std::shared_ptr<Data> data_;

 public:
  Cache(const Object& obj) : obj_(obj), data_(nullptr) {
    LOG("Cache for object " << typeid(Object).name() << std::endl
                            << "   inheriting from "
                            << typeid(Parent<Object>).name()
                            << std::endl << "   object result_type "
                            << typeid(typename Object::result_type).name()
                            << std::endl << "   cache result_type "
                            << typeid(result_type).name() << std::endl);
  }

  //! Get an up-to-date version of the cached object's get() return value
  const result_type& get() const {
    if (!data_) {  // first call
      data_ = std::make_shared<Data>(obj_);
    } else {
      unsigned version = obj_.get_version();
      if (version != data_->get_version()) {
        LOG("   Cache invalid, updating from version "
            << data_->get_version() << " to version " << version << std::endl);
        data_->set_value(obj_);
      }
    }
    return data_->get();
  }

  //! Get the current cache's version. Compare to
  //Object::get_version() to know if the next call to get() will invalidate the
  //cache.
  unsigned get_version() const {
    if (!data_) {  // first call
      data_ = std::make_shared<Data>(obj_);
    }
    return data_->get_version();
  }
};
}
}
#endif /* INTERNAL_CACHE_H */
