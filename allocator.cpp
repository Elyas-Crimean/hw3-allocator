
#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <cstdlib>

unsigned fact(unsigned a)
{
    if(a>12){
        std::cerr <<"Limit exceeded"<<std::endl;
        exit(1);
    }
    unsigned r = 1;
    for(unsigned i=2; i <=a;++i){
        r*=i;
    }
    return r;
}

template<class T, int N>
struct reserved_allocator
{
    using value_type = T;
    using type = T;
    using pointer = T*;
    using const_pointer = T const*;
    using reference = T&;
    using const_reference = T const&;
    reserved_allocator()
    {
        reserved_allocator::reserv = N;
        auto bp = (T*)std::malloc(reserv*sizeof(T));
        if(bp){
            base_pointer.push_back(bp);
            reserved_count = reserv;
            used_count = 0;
            std::cout << bp;
        }
    };
    ~reserved_allocator()
    {
        for(auto p : base_pointer){
           free(p);
       }
    };
      
    template<class Other>
    struct rebind{typedef reserved_allocator<Other, N> other;};

    T* allocate(std::size_t n)
    {
        if(used_count==reserved_count){
            auto bp = (T*)std::malloc(reserv*sizeof(T));
            if(bp){
                base_pointer.push_back(bp);
            }
            reserved_count+=reserv;
        }
        used_count++;
        return base_pointer[(used_count-1)/reserv] + (used_count-1)%reserv;
    };
    void deallocate(T* p, std::size_t n)
    {
    };
    template<typename U, typename ... Args>
    void construct(U* p, Args&& ... args)
    {
        new(p) U(std::forward<Args>(args) ... );
    };
    void destroy(T* p)
    {
        p->~T();
    };
    private:
    int reserved_count;
    int used_count;
    std::vector<T*> base_pointer;
//    T* base_pointer;
    int reserv;
};

template<class T>
struct Cell
{
    T value;
    Cell* next;
};

template<class T,
         class Allocator = std::allocator<T>
        > class My_list
{
    public:
    class iterator
    {
        public:
        iterator(){item=nullptr;}
        iterator(Cell<T> *cell){item=cell;}
        iterator(const iterator &it){item=it.item;}
        bool operator==(const iterator &it) const
        {return item == it.item;}
        bool operator!=(const iterator &it) const
        { return !(it == *this); }
        iterator& operator++()
        {   if ((item!=nullptr)&&(item->next!=nullptr))
            item = item->next;
            return *this;
        }
        T& operator*() const
        {if(item->next != nullptr) return item->value;
            else
            exit(1);
        }
        private:
        Cell<T> *item;
    };

    My_list()
    {
        empty.next = nullptr;
        top = last = &empty;
        top_i = iterator(top);
        end_i = iterator(&empty);
    }    
    void push_back(const T new_value)
    {
        if(&empty != top){
             last = last->next = a.allocate(sizeof(Cell<T>));
        }else{
            top = a.allocate(sizeof(Cell<T>));
            last = top;
            top_i = iterator(top);
        }
        last->value = new_value;
        last->next = &empty;
    }
    iterator begin() {return top_i;}
    iterator end() {return end_i;}
    
    private:
    typedef typename Allocator::template rebind<Cell<T>>::other Cell_a;
    Cell_a a;
    Cell<T> *top;
    Cell<T> *last;
    Cell<T> empty;
    iterator top_i;
    iterator end_i;
};


int main(int argc, char **argv)
{
    std::map<unsigned int,unsigned int> default_allocator_map;
    for(int i=0; i<10;++i){
        default_allocator_map[i]=fact(i);   
    }
/*    for(auto p : default_allocator_map){
        std::cout <<p.first<<" "<<p.second<<std::endl;
    }
*/    
    std::map<unsigned int,unsigned int,
             std::less<unsigned int>,
             reserved_allocator<std::pair<const unsigned int, unsigned int>,10>
            > my_allocator_map;
    for(int i=0; i<10;++i){
        my_allocator_map[i]=fact(i);   
    }
    for(auto p : my_allocator_map){
        std::cout <<p.first<<" "<<p.second<<std::endl;
    }
    
    My_list<int> my_std_list;
    for(int i=0; i<10;++i){
        my_std_list.push_back(i);
    }
    My_list<int,reserved_allocator<int,7>> my_custom_list;
    for(int i=0; i<10;++i){
        my_custom_list.push_back(i);
    }
    for(auto p:my_custom_list){
        std::cout << p << std::endl;
    }

	return 0;
}

