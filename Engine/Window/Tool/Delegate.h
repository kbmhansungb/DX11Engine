#pragma once
#include <unordered_map>
#include <unordered_set>
#include <list>
//using namespace std;

namespace Delegate
{
	template<typename... Arg>
	class delegate;
	class delenode;

	/// <summary>
	/// 
	/// </summary>
	class delenode
	{
	public:
		std::unordered_set<delegate<>*> useages;
		~delenode();
	};

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="...Arg"></typeparam>
	template<class... Arg>
	class delegate final : public delenode
	{
		// delenode,		this		func
		std::unordered_map <delenode*, std::pair<void*, std::list<void(delenode::*)(Arg...)> > > invokers;
	public:
		~delegate()
		{
			//delenode::~delenode();

			delegate<>* _this_delegate = (delegate<>*)this;
			delenode* delenode;
			for (auto it = invokers.begin(); it != invokers.end(); ++it)
			{
				delenode = it->first;
				delenode->useages.erase(_this_delegate);
			}
		}
		void Invoke(Arg... arg)
		{
			if (invokers.size() == 0) return;

			for (
				auto delenode_map_pair = invokers.begin(); 
				delenode_map_pair != invokers.end(); 
				++delenode_map_pair)
			{
				for (
					auto this_function_pair = delenode_map_pair->second.second.begin(); 
					this_function_pair != delenode_map_pair->second.second.end(); 
					++this_function_pair)
				{
					delenode* _this = (delenode*)delenode_map_pair->second.first;
					void(delenode::* _func)(Arg...) = *this_function_pair;
					(_this->*_func)(arg...);
				}
			}
		}
#pragma warning (disable : 4407)
		// C4407 �ٸ� ���� �����͸� ĳ��Ʈ�Ҷ� ȣ���.
		// CLASS_A::*func -> CLASS_B::*func�� ĳ���� �ؼ� ����ϱ� ������ ȣ���.

		template <typename Type, typename CastType>
		void AddInvoker(Type* context, void(CastType::* func)(Arg...))
		{
			AddInvoker(static_cast<CastType*>(context), func);
		}
		template<class Type>
		void AddInvoker(Type* context, void(Type::* func)(Arg...) )
		{
			// �ڱ��ڽ����״� ��������Ʈ�� �� �� ����.
			if (dynamic_cast<delenode*>(this) == dynamic_cast<delenode*>(context)) return;

			delenode* _node = dynamic_cast<delenode*>(context);
			void* _this = (void*)context;
			void(delenode:: * _func)(Arg...) = (void(delenode::*)(Arg...))func;
			delegate<>* _this_delegate = (delegate<>*)this;

			auto delenode_thisfunc_pair = invokers.find(_node);
			if (delenode_thisfunc_pair != invokers.end())
			{
				std::list<void(delenode::*)(Arg...)>& copy_cat = (delenode_thisfunc_pair->second.second);
				// �ִ� ���
				auto list_it = std::find(copy_cat.begin(), copy_cat.end(), _func);
				if (list_it == copy_cat.end())
				{
					copy_cat.push_back(_func);
				}
			}
			else
			{
				// ���� ���
				std::list<void(delenode::*)(Arg...)> Insert_list = { _func };
				invokers[_node] = std::make_pair(_this, Insert_list);
				_node->useages.insert(_this_delegate);
			}
		}
		template<class Type>
		void RemoveInvoker(Type* context, void(Type::* func)(Arg...))
		{
			delenode* _node = dynamic_cast<delenode*>(context);
			void* _this = (void*)context;
			void(delenode:: * _func)(Arg...) = (void(delenode::*)(Arg...))func;
			delegate<>* _this_delegate = (delegate<>*)this;

			bool remove = false;
			auto delenode_thisfunc_pair = invokers.find(_node);
			if (delenode_thisfunc_pair != invokers.end())
			{
				std::list<void(delenode::*)(Arg...)>& copy_cat = (delenode_thisfunc_pair->second.second);
				// �Լ��� ��ϵ� ��쿡�� �����Ѵ�.
				auto list_it = std::find(copy_cat.begin(), copy_cat.end(), _func);
				if (list_it != copy_cat.end())
				{
					copy_cat.erase(list_it);
				}

				if (copy_cat.size() == 0)
					remove = true;
			}
			// ���ŵ� ��쿡�� �۵��Ѵ�.
			if (remove)
			{
				invokers.erase(delenode_thisfunc_pair);
				_node->useages.erase(_this_delegate);
			}
		}
#pragma warning (default : 4407)
		void RemoveNode(delenode* context)
		{
			delenode* _node = dynamic_cast<delenode*>(context);
			delegate<>* _this_delegate = (delegate<>*)this;
			auto delenode_thisfunc_pair = invokers.find(_node);
			if (delenode_thisfunc_pair != invokers.end())
			{
				invokers.erase(delenode_thisfunc_pair);
				_node->useages.erase(_this_delegate);
			}
		}
	};
}

//class SampleClass : public Delegate::delenode
//{
//public:
//	void function(int a, int b) {};
//};
//
//void test()
//{
//	SampleClass sample;
//	Delegate::delegate<int, int> del;
//	del.AddInvoker(&sample, &SampleClass::function);
//	del.Invoke(3, 3);
//}