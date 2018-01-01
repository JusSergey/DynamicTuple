# DynamicTuple
It is a dynamic array which free types.
It is container, which cat containting objects different types, and different lenght.

# SampleCode
int main() {
    DynamicTuple dynt;
    dynt.emplace<int>(8);
    dynt.emplace<string>("Hello, world");

    std::initializer_list<int> some_vec{1, 4, 3, 5};
    dynt.emplace<vector<int>>(some_vec);

    int i = 0;
    cout << "int: " << dynt.get<int>(i++) << '\n';
    cout << "string: " << dynt.get<string>(i++) << '\n';

    for (int k : dynt.get<vector<int>>(i))
        cout << "vec elem: " << k << '\n';
    cout << '\n';
  
    return 0
}
# Output
int: 8
string: Hello, world
vec elem: 1
vec elem: 4
vec elem: 3
vec elem: 5
