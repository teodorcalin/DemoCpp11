#include <memory> // for unique_ptr
#include <iostream> // for cout
#include <typeinfo> // for typeid
#include <cxxabi.h> // for __cxa_demangle
#include <string> // for string, to_string

using namespace std;

class MyClass
{
public:
    explicit MyClass(const std::string& name = "default")
    :_name(name)
    {
        static unsigned int counter = 0;
        _name += to_string(counter++);
        cout << "Created " << _get_classname() << " object " << _name << endl;
    }

    void greet()
    {
        cout << "Hello from object " << _name << endl;
    }

    ~MyClass()
    {
        cout << "Destroyed " << _get_classname() << " object " << _name << endl;
    }
private:
    string _name;
    const char* _get_classname() const
    {
        int status;
        return abi::__cxa_demangle(typeid(*this).name(), 0, 0, &status);
    }
};


void demo_smart_pointers()
{
    cout << endl << "*************** Smart Pointers ***********" << endl;

    // Using unique_ptr with a single instance
    unique_ptr<MyClass> pObj(new MyClass("single"));
    pObj->greet();

    // Using unique_ptr with an array
    unique_ptr<MyClass[]> aObj(new MyClass[3]);

    // Using shared_ptr
    shared_ptr<MyClass> pSharedObj1, pSharedObj2;

    { // Different scope, for instance a function called from this scope
        cout << "Scope A started" << endl;
        shared_ptr<MyClass> pSharedObj3 = make_shared<MyClass>("shared");
        pSharedObj3->greet();

        // Share the object using other shared_ptr's
        pSharedObj1 = pSharedObj2 = pSharedObj3;

        // Display reference count to the object
        cout << "Object has " << pSharedObj3.use_count() << " references" << endl;

        // Therefore object will not be destroyed when pSharedObj3 goes out of scope
        cout << "Scope A ended" << endl;
    }
    pSharedObj1->greet();
    // Display reference count to the object
    cout << "Object has " << pSharedObj1.use_count() << " references" << endl;
    // Explicitely destroy one reference
    pSharedObj2.reset();
    cout << "Object has " << pSharedObj1.use_count() << " references" << endl;

    // Releasing from a unique_ptr
    MyClass* pUnmanagedObj(nullptr);
    { // Different scope, like a factory method
        cout << "Scope B started" << endl;
        unique_ptr<MyClass> pUniqueObj1(new MyClass("scoped"));
        unique_ptr<MyClass> pUniqueObj2(new MyClass("released"));
        // Release object so that it will not be deleted at end of scope
        pUnmanagedObj = pUniqueObj2.release();
        cout << "Scope B ended" << endl;
    }
    pUnmanagedObj->greet();
    delete pUnmanagedObj;

    // Using shared_ptr to manage pointers not allocated with new
    // Allocate memory with malloc
    MyClass* pMemForObj = reinterpret_cast<MyClass*>(malloc(sizeof(MyClass)));
    // Construct with placement new
    new (pMemForObj) MyClass("malloced");
    // Manage using a shared_ptr with custom deleter
    shared_ptr<MyClass> pMallocedObj(pMemForObj, [](MyClass* ptr){
        ptr->~MyClass();
        free(ptr);
    });
    pMallocedObj->greet();

    cout << "End of Smart Pointers demo" << endl;
}
