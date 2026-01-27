#pragma once 

template <typename RawType>
class glfw_adapter {
public:
	inline RawType get_raw_attribute() const { return m_raw_attribute; };
	inline RawType const cget_raw_attribute() const { return m_raw_attribute; };
protected:
	RawType m_raw_attribute;
};

