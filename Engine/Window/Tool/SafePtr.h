#pragma once

template <typename Type>
struct PtrNode final
{
	/// <summary>
	/// 참조하는 SafePtr이 없을 때 Ptr_node를 제거하는 역활을 한다.
	/// </summary>
	int Use_count;
	Type* Ptr;
};

template <typename Type>
class SafePtr final
{
	PtrNode<Type>* Ptr_node;

	void decrease()
	{
		if (Ptr_node != nullptr)
		{
			Ptr_node->Use_count--;
			if (Ptr_node->Use_count <= 0)
				delete Ptr_node;
			Ptr_node = nullptr;
		}
	}
	void increase()
	{
		if (Ptr_node != nullptr)
		{
			Ptr_node->Use_count++;
		}
	}
public:
	SafePtr() : Ptr_node(nullptr) {}
	SafePtr(Type* Ptr) : Ptr_node(Ptr->get_safe_ptr().cast<Type>().Ptr_node) {
		increase();
	}
	SafePtr(const SafePtr& ar) : Ptr_node(ar.Ptr_node) //복사생성자는 const를 꼭 붙여줘야 한다?
	{
		increase();
	}
	SafePtr(SafePtr&& rhs) : Ptr_node(rhs.Ptr_node) 
	{
		increase();
	}
	SafePtr(PtrNode<Type>* Ptr_node) : Ptr_node(Ptr_node)
	{
		increase();
	}
	~SafePtr()
	{
		decrease();
	}
	void reset()
	{
		decrease();
		Ptr_node = nullptr;
	}

	bool is_vaild()
	{
		if (Ptr_node == nullptr) return false;
		if (Ptr_node->Ptr == nullptr) return false;
		return true;
	}
	void operator= (const SafePtr<Type>& rhs)
	{
		decrease();
		Ptr_node = rhs.Ptr_node;
		increase();
	}
	bool operator==(const SafePtr<Type>& other) const
	{
		return this->Ptr_node == other.Ptr_node;
	}
	Type* operator->()
	{
		return this->get();
	}
	Type* get()
	{
		if (Ptr_node != nullptr)
			return Ptr_node->Ptr;
		else
			return nullptr;
	}
	template <typename Other>
	bool set(SafePtr<Other> other)
	{
		*this = other.cast<Type>();
		return this->is_vaild();
	}
	template <typename CastType>
	SafePtr<CastType> cast()
	{
		if (Ptr_node != nullptr)
		{
			CastType* result = dynamic_cast<CastType*>(Ptr_node->Ptr);

			if (result != nullptr)
			{
				return SafePtr<CastType>((PtrNode<CastType>*)Ptr_node);
			}
			else
			{
				return SafePtr<CastType>();
			}
		}
		else
		{
			return SafePtr<CastType>();
		}
	}
	friend class UseSafePtr;
};

class UseSafePtr
{
	SafePtr<UseSafePtr> This_safe_ptr;
protected:
	UseSafePtr() 
	{
		This_safe_ptr.Ptr_node = new PtrNode<UseSafePtr>();
		This_safe_ptr.Ptr_node->Ptr = this;
		This_safe_ptr.Ptr_node->Use_count = 1;
	}
public:
	virtual ~UseSafePtr() 
	{
		This_safe_ptr.Ptr_node->Ptr = nullptr;
	} // 다형성을 위한 구조

	SafePtr<UseSafePtr>& get_safe_ptr()
	{
		return This_safe_ptr;
	}
	//template<typename CastType> 
	//SafePtr<UseSafePtr>& get_safe_ptr()
	//{
	//	return get_safe_ptr().cast<CastType>();
	//}
};

//class TestClass : public UseSafePtr
//{
//};
//void test()
//{
//	TestClass sample;
//	SafePtr<TestClass> test = sample.get_safe_ptr().cast<TestClass>();
//}
//void test2(SafePtr<TestClass> test)
//{
//	if (test.is_vaild())
//	{
//		// func
//	}
//}
