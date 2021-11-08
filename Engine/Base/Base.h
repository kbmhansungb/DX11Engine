#pragma once

#include "../_Include.h"

class Base : public delenode, public UseSafePtr
{
public:
	virtual std::type_index get_class_type() { return typeid(Base); }
	Base();
	virtual ~Base();

	virtual void draw_detail_view();

	template <typename Payload>
	void serialize(Payload* payload)
	{
	}
};