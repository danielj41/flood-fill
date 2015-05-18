#ifndef POINTER_MACROS_H
#define POINTER_MACROS_H

#include <memory>

#define DEF_PTR(type, ptrType) class type; typedef std::shared_ptr<type> ptrType
#define DEF_PTR_TMPL(type, ptrType, tmpl, tmplId) tmpl class type; tmpl using ptrType = type <tmplId>

#define PTR_CAST(type, value) (std::dynamic_pointer_cast<type>(value))

#define NULL_PTR nullptr

#endif