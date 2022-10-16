#include <jsapi.h>
#include <iostream>

#include <js/Initialization.h>
#include <js/Exception.h>

#include "boilerplate.hpp"

JSObject* boilerplate::CreateGlobal(JSContext* cx) {
  JS::RealmOptions options;

  static JSClass BoilerplateGlobalClass = {
      "BoilerplateGlobal", JSCLASS_GLOBAL_FLAGS, &JS::DefaultGlobalClassOps};

  return JS_NewGlobalObject(cx, &BoilerplateGlobalClass, nullptr,
                            JS::FireOnNewGlobalHook, options);
}


void boilerplate::ReportAndClearException(JSContext* cx) {
  JS::ExceptionStack stack(cx);
  if (!JS::StealPendingExceptionStack(cx, &stack)) {
    fprintf(stderr, "Uncatchable exception thrown, out of memory or something");
    exit(1);
  }

  JS::ErrorReportBuilder report(cx);
  if (!report.init(cx, stack, JS::ErrorReportBuilder::WithSideEffects)) {
    fprintf(stderr, "Couldn't build error report");
    exit(1);
  }

  JS::PrintError(stderr, report, false);
}

bool boilerplate::RunExample(bool (*task)(JSContext*, int argc, const char* argv[]), int argc, const char* argv[], bool initSelfHosting) {
  if (!JS_Init()) {
    return false;
  }

  JSContext* cx = JS_NewContext(JS::DefaultHeapMaxBytes);
  if (!cx) {
    return false;
  }
  
  if (initSelfHosting && !JS::InitSelfHostedCode(cx)) {
    return false;
  }

  if (!task(cx, argc, argv)) {
    return false;
  }

  JS_DestroyContext(cx);
  JS_ShutDown();

  return true;
}