#ifndef __OBJECT__
#define __OBJECT__

#include <memory>

class Object
{
protected:
	std::shared_ptr<void> m_value;
public:
	Object(std::shared_ptr<void> value)
	{
		m_value = value;
	}
	std::shared_ptr<void> getValue()
	{
		return m_value;
	}
};

#define VALUE(value) Object(std::shared_ptr<void>(&value))

#endif /** __OBJECT__ **/