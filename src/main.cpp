#include <cstdio>

#include <map>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>

#include <jsapi.h>
#include <js/CompilationAndEvaluation.h>
#include <js/Modules.h>
#include <js/SourceText.h>

#include "boilerplate.hpp"

std::string readFileIntoString(const std::string& path) {
    std::ifstream input_file(path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '"
             << path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    return std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

static bool raw_write(JSContext* cx, unsigned argc, JS::Value* vp) {

  JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
  JS::RootedString value(cx, args.get(0).toString());
  printf("%s\n", JS_EncodeStringToUTF8(cx, value).get());
  args.rval().setNull();

  return true;
}

static bool DefineGlobalFunction(JSContext* cx, JS::HandleObject global) {
  if (!JS_DefineFunction(cx, global, "raw_write", &raw_write, 1, 0))
    return false;
  return true;
}

static bool ExecuteCode(JSContext* cx, const char* code) {
  JS::CompileOptions options(cx);
  options.setFileAndLine("noname", 1);

  JS::SourceText<mozilla::Utf8Unit> source;
  if (!source.init(cx, code, strlen(code), JS::SourceOwnership::Borrowed)) {
    return false;
  }

  JS::RootedValue unused(cx);

  return JS::Evaluate(cx, options, source, &unused);
}

static bool HelloExample(JSContext* cx, int argc, const char* argv[]) {
  JS::RootedObject global(cx, boilerplate::CreateGlobal(cx));
  if (!global) {
    return false;
  }

  JSAutoRealm ar(cx, global);

  if (!DefineGlobalFunction(cx, global)) {
    return false;
  }

  JS::RootedValue f(cx);
  JSFunction* newFunction = JS_NewFunction(cx, raw_write, 0, 0, "f");
  if (!newFunction) return false;
  f.setObject(*JS_GetFunctionObject(newFunction));

  if (argc < 2) {
    printf("Err: missing file\n");
    return false;
  }

  std::string filename(argv[1]);
  std::string file_contents;

  file_contents = readFileIntoString(filename);
  auto nevim = ExecuteCode(cx, file_contents.c_str());
  return nevim;
}

int main(int argc, const char* argv[]) {
  if (!boilerplate::RunExample(HelloExample, argc, argv)) {
    return 1;
  }
  return 0;
}