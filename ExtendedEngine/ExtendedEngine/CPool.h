#pragma once
#include <list>
#include <memory>

template <class T>
class CPool
{
private:
	//Custom deleter for the unique_ptr that will return the resource back to the pool if the pool exists, otherwise deletes it.
	struct ResourceDeleter
	{
	private:
		std::weak_ptr<CPool<T>> mPool;

	public:
		explicit ResourceDeleter(std::weak_ptr<CPool<T>> pool) : mPool(pool) {}
		explicit ResourceDeleter() {}

		void operator()(T* ptr) //Deletion function
		{
			if (std::shared_ptr<CPool<T>> pool_ptr = mPool.lock())
			{
				(pool_ptr).get()->AddRes(ptr);	//Add the resource back to the pool if the pool exist
			}
			else
			{
				std::default_delete<T>{}(ptr);	//Delete the resource if there is no pool to return it to
			}
		}
	};

	static std::shared_ptr<CPool<T>> mPool;

	std::list<std::unique_ptr<T>> mResources;

	CPool() {}
public:
	//Pointer type
	using resource_ptr = std::unique_ptr<T, ResourceDeleter>;

	//Create pool's objects
	void Init(int size)
	{
		for (int i = 0; i < size; i++)
		{
			AddRes(new T);
		}
	}

	void Clear()
	{
		mResources.clear(); //Unique pointers will handle the deletion
	}

	resource_ptr GetRes()
	{
		if (mResources.empty())
		{
			AddRes(new T);
		}
		resource_ptr ptr(mResources.back().release(),
			ResourceDeleter{ std::weak_ptr<CPool>(mPool) });
		ptr.get()->Reset();
		mResources.pop_back();
		return move(ptr);
	}

	void AddRes(T* resource)
	{
		resource->Hide();
		unique_ptr<T> res(resource);
		mResources.push_back(move(res));
	}

	static CPool<T>* GetInstance()
	{
		if (!(mPool.get())) //Check if exists
		{
			if (!(mPool.get())) //Create if doesn't exist
			{
				mPool.reset(new CPool);
			}
		}
		return mPool.get();
	}


	~CPool()
	{
		mResources.clear(); //Unique pointers will handle the deletion
	}
};

//A more short form type name to avoid having to put the CPool<T>::
//Also makes it written in the same way a unique_ptr
// eg. res_ptr<T> is more comparable to unique_ptr<T> than CPool<T>::resource_ptr
template<class T>
using res_ptr = typename CPool<T>::resource_ptr;