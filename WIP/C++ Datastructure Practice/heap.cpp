#include <iostream>
#include <iomanip>
#include <functional>
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <math.h>
#include <utility>
#include <vector>
#include <map>
#define get_time duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()

using namespace std;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;

typedef struct functiontype{
    string name;
    function<int(long double)> unary_function;
} ft_t;

void test(function<bool(int, int)> comp, int);
bool omega_test(function<void(int)>, vector<ft_t>, int, int, int);
void test_push_back(int n);
void test_constant(int n);

// class T must override the < and > operators in order for min_heap to work.
template<class T> class heap{
    private:
        int n;
        int chunk;
        int max_sz;
        T* a;
        function<bool(T, T)> comp;

        void swap(int i, int j){
            T tmp = *(a + j);
            *(a + j) = *(a + i);
            *(a + i) = tmp;
        }


    public:
        heap(int chunk = 100){
            this->n = 0;
            this->chunk = chunk;
            this->max_sz = chunk;
            this->a = (T*) malloc(sizeof(T) * max_sz);
            this->comp = [](T x, T y) -> bool {return x < y;};
        }

        heap(function<bool(T,T)> comp, int chunk = 100){
            this->n = 0;
            this->chunk = chunk;
            this->max_sz = chunk;
            this->a = (T*) malloc(sizeof(T) * max_sz);
            this->comp = comp;
        }

        ~heap(){
            free(a);
        }

        void copy(T* ds, int n){
            for(int i = 0; i < n; ++i){
                push(*(ds + i));
            }
        }

        void push(T var){
            // we need to re-allocate the array!
            if(n >= max_sz){
                T* a2 = (T*) malloc(sizeof(T) * (max_sz + chunk));
                max_sz += chunk;

                for(int i = 0; i < n; ++i){
                    *(a2 + i) = *(a + i);
                }

                *(a2 + n) = var;

                free(a);
                a = a2;
            }else{
                *(a + n) = var;
            }

            

            // now move [n - 1] down
            int j = n;
            int i = (j - 1)/2;
            while(j > 0){
                if(comp(*(a + j), *(a + i))){
                    swap(i, j);
                    j = i;
                    i = (j - 1)/2;
                }else{
                    break;
                }
            }

            // n has now grown.
            n++;
        }

        T pop(){
            if(n == 0){
                return 0;
            }

            T tmp = a[0];
            swap(0, n - 1);

            n--;

            int i = 0;
            int j = 1;
            while(j < n){
                if(comp(*(a + j), *(a + i)) && (j + 1 >= n || comp(*(a + j), *(a + j + 1)) || *(a + j) == *(a + j + 1))){
                    swap(j, i);
                    i = j;
                    j = 2 * i + 1;
                }else if(j + 1 < n && comp(*(a + j + 1), *(a + i))){
                    swap(i, j + 1);
                    i = j + 1;
                    j = 2 * i + 1;
                }else{
                    break;
                }
            }

            return tmp;
        }
        
        int size(){
            return n;
        }

        void print_heap(){
            cout << "heap: ";
            for(int i = 0; i < n; ++i){
                cout << *(a + i) << " ";
            }
            cout << "\n";
        }
};

int main(void){
    test([](int x, int y) -> bool {return x < y;}, 100000);
    test([](int x, int y) -> bool {return x > y;}, 100000);
    function<void(int)> test_func = [](int n)->void{test([](int x, int y) -> bool {return x < y;}, n);};
    vector<ft_t> functions;

    ft_t constant;
    constant.name = "O(1)";
    constant.unary_function = [](int n)->double {return 1;};
    functions.push_back(constant); 

    ft_t logarithmic;
    logarithmic.name = "O(log n)";
    logarithmic.unary_function = [](int n)->double {return log(n);};
    functions.push_back(logarithmic); 

    ft_t sqrt;
    sqrt.name = "O(sqrt(n))";
    sqrt.unary_function = [](int n)->long double {return pow(n, 0.5);};
    functions.push_back(sqrt); 

    ft_t linear;
    linear.name = "O(n)";
    linear.unary_function = [](int n)->long double {return n;};
    functions.push_back(linear); 

    ft_t linearxlogarithmic;
    linearxlogarithmic.name = "O(n log n)";
    linearxlogarithmic.unary_function = [](int n)->long double {return n * log(n);};
    functions.push_back(linearxlogarithmic); 

    ft_t quadratic;
    quadratic.name = "O(n^2)";
    quadratic.unary_function = [](int n)->long double {return 0.001 * n * n;};
    functions.push_back(quadratic); 

    ft_t exponential;
    exponential.name = "O(2^n)";
    exponential.unary_function = [](int n)->long double {return pow(1.001, n);};
    functions.push_back(exponential); 

    ft_t super_exponential;
    super_exponential.name = "O(n^n)";
    super_exponential.unary_function = [](int n)->long double {return pow(n, 0.0001 * n);};
    functions.push_back(super_exponential); 

    omega_test(test_push_back, functions, 500000, 10000, 10000);
}

void test_push_back(int n){
    srand(10);

    function<bool(int,int)> comp = [](int x, int y) -> bool {return x > y;};
    heap<int> mh(comp, 100);

    // cout << "adding random elements: " << endl;
    for(int i = 0; i < n; ++i){
        mh.push(rand() % 1000000);
    }
}

void test_constant(int n){
    function<bool(int,int)> comp = [](int x, int y) -> bool {return x > y;};
    heap<int> mh(comp, n);

    // cout << "adding random elements: " << endl;
    for(int i = 0; i < 1000000; ++i){
        mh.push(rand() % 1000000);
    }
}

void test(function<bool(int,int)> comp, int n){
    srand(10);
    int arr[n];
    for(int i = 0; i < n; ++i){
        arr[i] = rand() % 1000000;
    }

    // cout << "init: " << endl;
    heap<int> mh(comp, 500000);
    
    // cout << "copying: " << endl;
    mh.copy(arr, sizeof(arr) / sizeof(int));

    // cout << "adding random elements: " << endl;
    for(int i = 0; i < n; ++i){
        mh.push(rand() % 1000000);
    }

    // cout << "test all elements are ordered: " << endl;
    int i = mh.pop();
    // cout << i << " ";
    while(mh.size() > 0){
        int j = mh.pop();
        // cout << j << " ";
        if(!(comp(i, j) || i == j)) {
            cout << "\nERROR at index " << i << "\n";
        }
        // cout << j << " ";
        i = j;
    }
    // cout << "\n";
}

bool omega_test(function<void(int)> func, vector<ft_t> fs, int n, int st=0, int jmp=1){
    int num_functions = fs.size();

    map<int, long long> n_to_duration;

    long long bf;
    long long af;
    int duration;
    long long total_time = 0;
    int count = 0;

    for(int i = st; i <= n; i += jmp){
        bf = get_time;
        func(i);
        af = get_time;
        duration = af - bf;
        n_to_duration[i] = duration;
        total_time += duration;
        cout << "(n:" << setw(8) << i << ", Time:" << setw(8) << (double) duration / 1000 << "s)\n";
        count++;
    }
    cout << "\n";
    cout << "Total time: " << (double) total_time / 1000 << "\n\n";

    auto at = n_to_duration.begin();
    int interval = 11;

    long double ratios[num_functions][count];
    cout << "Ratio Table:\n";
    cout << left << setw(12) << " ";
    for(int i = 0; i < num_functions; ++i){
        cout << setw(20) << fs[i].name << setw(2);
    }
    cout << "\n";

    int c = 0;
    auto it_begin = n_to_duration.begin();

    double normalize_vals[num_functions];
    for(int i = 0; i < num_functions; ++i){
        normalize_vals[i] = (long double) it_begin->second / fs[i].unary_function(it_begin->first);
        if(normalize_vals[i] == 0){
            for(auto it = ++n_to_duration.begin(); it != n_to_duration.end(); ++it){
                if((long double) it->second / fs[i].unary_function(it->first) != 0){
                    normalize_vals[i] = (long double) it->second / fs[i].unary_function(it->first);
                    break;
                }
            }
        }
    }
    for(auto it = n_to_duration.begin(); it != n_to_duration.end(); ++it){
        cout << setw(10) << it->first << setw(2);
        for(int i = 0; i < num_functions; ++i){
            ratios[i][c] = (long double) it->second / fs[i].unary_function(it->first);
            ratios[i][c] /= normalize_vals[i];
            cout << setw(20) << setprecision(5) << fixed << ratios[i][c] << setw(2);
        }
        c++;
        cout << "\n";
    }

    long double dratios[num_functions][count - 1];
    cout << "\nRatio diff Table:\n";
    cout << setw(12) << " ";
    for(int i = 0; i < num_functions; ++i){
        cout << setw(20) << fs[i].name << setw(2);
    }
    cout << "\n";

    c = 0;
    for(int i = 0; i < num_functions; ++i){
        normalize_vals[i] = abs(ratios[i][1] - ratios[i][0]) / jmp;
        if(normalize_vals[i] == 0){
            for(int j = 2; j < count; ++j){
                if(abs(ratios[i][j] - ratios[i][j-1]) / jmp != 0){
                    normalize_vals[i] = abs(ratios[i][j] - ratios[i][j-1]) / jmp;
                    break;
                }
            }
        }
    }

    for(auto it = ++n_to_duration.begin(); it != n_to_duration.end(); ++it){
        cout << setw(10) << it->first << setw(2);
        for(int i = 0; i < num_functions; ++i){
            dratios[i][c] = abs(ratios[i][c + 1] - ratios[i][c]) / jmp;
            dratios[i][c] /= normalize_vals[i];
            cout << setw(20) << setprecision(5) << dratios[i][c] << setw(2);
        }
        c++;
        cout << "\n";
    }

    vector<int> vals[num_functions];
    for(int i = 0; i < num_functions; ++i){
        for(int j = 0; j < count - 1; ++j){
            vals[i][j] = dratios[i][j];
        }
    }

    for(int i = 0; i < num_functions; ++i){
        sort(vals[i].begin(), vals[i].end());
    }

    double medians[num_functions];
    for(int i = 0; i < num_functions; ++i){
        medians[i] = vals[num_functions][num_functions/2];
    }

    // long double ddratios[num_functions][count - 2];
    // cout << "\nRatio diff^2 Table:\n";
    // cout << setw(12) << " ";
    // for(int i = 0; i < num_functions; ++i){
    //     cout << setw(20) << fs[i].name << setw(2);
    // }
    // cout << "\n";

    // c = 0;
    // for(auto it = ++(++n_to_duration.begin()); it != n_to_duration.end(); ++it){
    //     cout << setw(10) << it->first << " diff^2" << setw(2);
    //     for(int i = 0; i < num_functions; ++i){
    //         ddratios[i][c] = (dratios[i][c + 1] - dratios[i][c]) / jmp;
    //         cout << setw(20) << setprecision(5) << ddratios[i][c] << setw(2);
    //     }
    //     c++;
    //     cout << "\n";
    // }

    return true;
}