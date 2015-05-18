#include <memory>

#define DEF_PTR(type, ptrType) class type; typedef std::shared_ptr<type> ptrType
#define DEF_PTR_TMPL(type, ptrType, tmpl, tmplId) tmpl class type; tmpl using ptrType = type <tmplId>