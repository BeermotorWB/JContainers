
namespace tes_api_3 {

    using namespace collections;

#if 1
    class tes_lua : public class_meta < tes_lua > {
    public:

        void additionalSetup() {
            metaInfo.comment = "Evaluates Lua code";
        }

#define ARGNAMES "luaCode transport default="
#define ARGNAMES_2 " minimizeLifetime=true"
        REGISTERF(evalLua<Float32>, "evalLuaFlt", ARGNAMES "0.0" ARGNAMES_2,
R"===(Evaluates piece of Lua code. The arguments are carried by @transport object.
The @transport is any kind of object, not just JMap.
If @minimizeLifetime is True the function will invoke JValue.zeroLifetime on the @transport object.
Returns @default value if evaluation fails.
Usage example:

    ; 7 from the end until 9 from the end. Returns "Lua" string
    string input = "Hello Lua user"
    string s = JLua.evaLuaStr("return string.sub(args.string, args.low, args.high)",\
        JLua.setObj("string",input, JLua.setInt("low",7, JLua.setInt("high",9 )))\
    )
)===");
        REGISTERF(evalLua<SInt32>, "evalLuaInt", ARGNAMES "0" ARGNAMES_2, nullptr);
        REGISTERF(evalLua<skse::string_ref>, "evalLuaStr", ARGNAMES R"("")" ARGNAMES_2, nullptr);
        REGISTERF(evalLua<Handle>, "evalLuaObj", ARGNAMES "0" ARGNAMES_2, nullptr);
        REGISTERF(evalLua<TESForm*>, "evalLuaForm", ARGNAMES "None" ARGNAMES_2, nullptr);
#undef ARGNAMES
#undef ARGNAMES_2

        template<class ResultType>
        static ResultType evalLua(const char* luaCode, object_base* transport, ResultType def, bool minimizeLifetime = true) {
            auto result = lua::eval_lua_function(tes_context::instance(), transport, luaCode);
            if (transport && minimizeLifetime) {
                transport->zero_lifetime();
            }
            return result ? result->readAs<ResultType>() : def;
        }

#define ARGNAMES "key value transport=0"
        REGISTERF(pushArg<const char*>, "setStr", ARGNAMES,
R"===(Inserts new (or replaces existing) {key -> value} pair. Expects that @transport is JMap object, if @transport is 0 it creates new JMap object.
Returns @transport)===");
        REGISTERF(pushArg<Float32>, "setFlt", ARGNAMES, "");
        REGISTERF(pushArg<SInt32>, "setInt", ARGNAMES, "");
        REGISTERF(pushArg<FormId>, "setForm", ARGNAMES, "");
        REGISTERF(pushArg<object_base*>, "setObj", ARGNAMES, "");
#undef ARGNAMES

        template<class ArgType>
        static map* pushArg(const char* key, ArgType arg, map* transport = nullptr) {
            if (!transport) {
                transport = &map::object(tes_context::instance());
            }

            tes_map::setItem<ArgType>(transport, key, arg);
            return transport;
        }

        REGISTER_TES_NAME("JLua");
    };

    TEST(JLua, simple)
    {
        EXPECT_EQ( 8, tes_lua::evalLua<float>("return args.x * args.y", tes_lua::pushArg("x", 2, tes_lua::pushArg("y", 4)), 0.f) );

        EXPECT_EQ(1, tes_lua::evalLua<SInt32>("return jobject ~= nil", tes_lua::pushArg("garbage", 4), -1))
            << "@jobject (@args alias) isn't supported";
    }
    TES_META_INFO(tes_lua);
#endif
}
 