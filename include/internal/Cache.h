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
#include <utility>

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
  typedef decltype(std::declval<EigenExpr>().eval()) eigen_eval_type;
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
  class CacheData {
   private:
    mutable bool first_call_;
    mutable unsigned version_;
    Object obj_; 
    mutable std::unique_ptr<result_type> val_;

   private:
    void set_value(const Object& obj, std::true_type) const {
      version_ = obj.get_version();
      // force evaluation (e.g. as matrix, not expr)
      val_.reset(new result_type(obj.get().eval()));
    }
    void set_value(const Object& obj, std::false_type) const {
      version_ = obj.get_version();
      // obj.get() does not have eval(): store as-is
      val_.reset(new result_type(obj.get()));
    }
    void set_value(const Object& obj) const { set_value(obj, eval_called()); }
    void init(const Object& obj) const {
      LOG("CacheData::init() : data init" << std::endl)
      set_value(obj);
    }

    CacheData(const CacheData&); // cannot be copied

   public:
    const result_type& get() const {
      if (!val_) {
        init(obj_);
      } else if (obj_.get_version() != get_version()) {
        LOG("Cache::get() : invalid, updating " << std::endl);
        set_value(obj_);
      }
      return *val_;
    }
    unsigned get_version() const {
      if (!val_) {
        init(obj_);
      }
      return version_;
    }

    CacheData(const Object& obj)
        : version_(0), obj_(obj), val_(nullptr) {
      LOG("CacheData init " << std::endl);
    }
  };

  mutable std::shared_ptr<CacheData> data_;

 public:
  Cache(const Object& obj) : data_(std::make_shared<CacheData>(obj)) {
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
    LOG("Cache::get() : returning data" << std::endl);
    return data_->get();
  }

  //! Get the current cache's version. Compare to
  //Object::get_version() to know if the next call to get() will invalidate the
  //cache.
  unsigned get_version() const {
    LOG("Cache::get_version() : returning data" << std::endl);
    return data_->get_version();
  }
};
}
}
#endif /* INTERNAL_CACHE_H */
