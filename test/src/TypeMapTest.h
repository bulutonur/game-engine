#ifndef TYPEMAP_TEST_H_
#define TYPEMAP_TEST_H_

#include "utest.h"

#include "TypeMap.h"


class A
{

};

class B : public A
{

};


class C : public A
{

};

class D
{

};

TEST(TypeMap, has)
{

	TypeMap<int> map;
	int* a = new int(1);
	int* b = new int(2);
	map.add<A>(a);
	map.add<B>(b);
	ASSERT_TRUE(map.has<A>());
	ASSERT_TRUE(map.has<B>());
	ASSERT_TRUE(map.hasNot<C>());
}

TEST(TypeMap, get)
{

	TypeMap<A> map;
	A* a = new A;
	B* b = new B;
	map.add<A>(a);
	map.add<B>(b);
	A* a2 = map.get<A>();
	B* b2 = map.get<B>();
	C* c = map.get<C>();
	ASSERT_NE(nullptr, a2);
	ASSERT_NE(nullptr, b2);
	ASSERT_EQ(nullptr, c);
}

TEST(TypeMap, iterator)
{

	TypeMap<A> map;
	A* a = new A;
	B* b = new B;
	map.add<A>(a);
	map.add<B>(b);
	int counter = 0;
	for (TypeMap<A>::Iterator it = map.begin(); it != map.end(); ++it)
	{
		counter++;
	}
	ASSERT_EQ(2, counter);
}


TEST(TypeMap, clear)
{

	TypeMap<A> map;
	A* a = new A;
	B* b = new B;
	map.add<A>(a);
	map.add<B>(b);

	map.clear();

	ASSERT_TRUE(map.hasNot<A>());
	ASSERT_TRUE(map.hasNot<B>());
}

#endif