#include <jsapi.h>

// See 'boilerplate.cpp' for documentation.

namespace boilerplate {

extern const JSClassOps DefaultGlobalClassOps;

JSObject* CreateGlobal(JSContext* cx);

void ReportAndClearException(JSContext* cx);

bool RunExample(bool (*task)(JSContext*, int argc, const char* argv[]), int argc, const char* argv[], bool initSelfHosting = true);

}  // namespace boilerplate