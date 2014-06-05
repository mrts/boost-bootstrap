#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/foreach.hpp>
#include <boost/type_traits.hpp>

#define foreach BOOST_FOREACH

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

struct Person
{
	string name;
    Person(const string& n) : name(n) {}
	virtual ~Person() {}
};

ostream& operator<< (ostream& o, const Person& person)
{ return o << person.name; }

struct Developer : public Person
{
    Developer(const string& n) : Person(n) {}
	virtual ~Developer() {}
};

bool did_find_john(const vector<boost::shared_ptr<Person> >& items,
		boost::function<string (const boost::shared_ptr<Person>&)> get_field)
{
	return find_if(items.begin(), items.end(),
			boost::bind(get_field, _1) != "John") != items.end();
}

template <typename Item, typename Result>
Result map_reduce(const vector<boost::shared_ptr<Item> >& items,
		boost::function<Result (const boost::shared_ptr<Item>&)> function)
{
	Result result;
	foreach (boost::shared_ptr<Item> item, items) {
		result += function(item);
	}
	return result;
}

template <typename T>
typename boost::enable_if<boost::is_pod<T>, void>::type
print(const T value)
{ cout << value << " is a POD type" << endl; }

template <typename T>
typename boost::disable_if<boost::is_pod<T>, void>::type
print(const T& value)
{ cout << value << " is not a POD type" << endl; }

int main()
{
	vector<boost::shared_ptr<Person> > persons;
	persons.push_back(boost::make_shared<Developer>("John"));
	persons.push_back(boost::make_shared<Person>("Joe"));

	cout << "Did find John? "
		<< (did_find_john(persons, &Person::name) ? "yes" : "no")
		<< endl;

	cout << "Concatenated usernames: "
		<< map_reduce<Person, string>(persons, &Person::name)
		<< endl;

	print(1);
	print(*persons[0]);

	return 0;
}
