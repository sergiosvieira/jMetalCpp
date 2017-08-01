#ifndef __VALUE__
#define __VALUE__

template <typename T>
class Value: public ValueBase
{
protected:
    T m_data;
public:
    Value(T data): m_data(data)
    {
        std::string name{typeid(T).name()};
        if (name.compare("int"))
        {
            m_type = ValueType::Integer;
        }
        else if (name.compare("double"))
        {
            m_type = ValueType::Double;
        }
        else if (name.compare("bool"))
        {
            m_type = ValueType::Bool;
        }
        else if (name.compare("float"))
        {
            m_type = ValueType::Float;
        }
        else if (name.compare("string"))
        {
            m_type = ValueType::String;
        }
        else
        {
            m_type = ValueType::Object;
        }
    };
    T getData()
    {
        return m_data;
    }
    ValueType getType()
    {
        return m_type;
    }
    void stream(std::ostream& os) override
    {
        os << m_data;
    }
};

#define MakeShared(type, value) std::make_shared<Value<type>>(value)
#define SharedPtr(type) std::shared_ptr<type>
#define SharedValue(type) SharedPtr(Value<type>)
#define IntValue(value) MakeShared(int, value)->getData()
#define DoubleValue(value) MakeShared(double, value)->getData()
#define BoolValue(value) MakeShared(bool, value)->getData()
#define FloatValue(value) MakeShared(float, value)->getData()
#define StringValue(value) MakeShared(std::string, value)->getData()

#define CastValue(value_ptr, type) std::static_pointer_cast<Value<type>>(value_ptr)
#define GetValue(value_ptr, type) CastValue(value_ptr, type)->getData()
#define GetIntValue(value_ptr) GetValue(value_ptr, int)
#define GetDoubleValue(value_ptr) GetValue(value_ptr, double)
#define GetBoolValue(value_ptr) GetValue(value_ptr, bool)
#define GetFloatValue(value_ptr) GetValue(value_ptr, float)
#define GetStringValue(value_ptr) GetValue(value_ptr, std::string)

#endif /** __VALUE__ **/