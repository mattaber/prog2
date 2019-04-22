#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <queue>

using namespace std;

typedef struct node {
    int profit, weight, level, bound;
} node;

struct node_comp {
    bool operator()(const node & a, const node & b) {
        return a.bound < b.bound;
    }
};

int knapsack(int, const int p[], const int w[], int, int);
int bound(int i, int n, int capacity, const int p[], const int w[], int weight, int profit);
bool pair_comp(const pair<int, int> &, const pair<int, int> &);
void solution_helper(int i, int s, int r, int n);

int leaf_count = -1, total_count = -1;
vector<int> solution;
vector<int> solution_final;
vector<int> x;

int
main(int argc, char ** argv) {
    string temp1, temp2, temp3;
    int n, c;
    vector< pair<int, int> > vec;

    fstream fs(argv[1], fstream::in);
    bool first = true;

    while(getline(fs, temp1)) {
        istringstream ss(temp1);
        if(first) {
            first = false;
            getline(ss, temp2, ',');
            n = stoi(temp2);
            getline(ss, temp2, ',');
            c = stoi(temp2);
        } else {
            getline(ss, temp2, ',');
            getline(ss, temp3, ',');
            pair<int, int> temp_pair(stoi(temp2), stoi(temp3));
            vec.push_back(temp_pair);
        }
    }

    fs.close();

    sort(vec.begin(), vec.end(), pair_comp);

    int w[vec.size()];
    int p[vec.size()];
    for(int i = 0; i < vec.size(); ++i) {
        w[i] = vec[i].first;
        p[i] = vec[i].second;
    }

    int max_profit = knapsack(n, p, w, c, 0);

    for(auto i : solution) {
        x.push_back(0);
    }

    solution_helper(0, 0, max_profit, n);

    fstream outf(argv[2], fstream::out);

    outf << n << "," << max_profit << "," << solution_final.size() << endl;
    outf << total_count << "," << leaf_count << endl;
    for(auto i : solution_final) {
        outf << w[i-1] << "," << p[i-1] << endl;
    }

    x.clear();
    solution.clear();
    solution_final.clear();
    vec.clear();

    outf.close();

    return 1;
}

int
knapsack(int n, const int p[], const int w[], int capacity, int max_profit) {
    priority_queue< node, vector<node>, node_comp > q;
    node u, v;
    u.level = v.level = 0;
    u.profit = v.profit = 0;
    u.weight = v.weight = 0;
    u.bound = v.bound = bound(v.level, n, capacity, p, w, v.weight, v.profit);
    q.push(v);

    while(!q.empty()) {
        v = q.top();
        q.pop();

        if(v.bound > max_profit) {
            u.level = v.level + 1;
            u.weight = v.weight + w[u.level - 1];
            u.profit = v.profit + p[u.level - 1];
        }

        if((u.weight <= capacity) && (u.profit > max_profit)) {
            max_profit = u.profit;
        }

        if((u.weight <= capacity) && bound(u.level, n, capacity, p, w, u.weight, u.profit) > max_profit) {
            q.push(u);
            solution.push_back(u.level);
        } else ++leaf_count;
        ++total_count;

        u.weight = v.weight;
        u.profit = v.profit;
        u.bound = bound(u.level, n, capacity, p, w, u.weight, u.profit);
        if((u.bound > max_profit)) {
            q.push(u);
        } else ++leaf_count;
        ++total_count;
    }
    return max_profit;
}

int
bound(int i, int n, int capacity, const int p[], const int w[], int weight, int profit) {
    if(weight >= capacity) return 0;
    int bound = profit;
    int j = i;
    int total_weight = weight;

    while((j < n) && (total_weight + w[j] <= capacity)) {
        total_weight += w[j];
        bound += p[j];
        ++j;
    }

    if(j < n) bound += (capacity - total_weight) * p[j] / w[j];

    return bound;
}

bool
pair_comp(const pair<int, int> & first, const pair<int, int> & second) {
    return (double)(first.second / first.first) > (double)(second.second / second.first);
}

void
solution_helper(int i, int s, int r, int n) {
    x[i] = 1;
    if(s <= n && r >= n - s && solution[i] <= n - s) {
        if(s + solution[i] == n) {
            for(int j = 0; j <= i; ++j) {
                if(x[j] == 1) {
                    solution_final.push_back(solution[j]);
                }
            }
        } else {
            solution_helper(i + 1, s + solution[i], r - solution[i], n);
        }
    }

    x[i] = 0;
    if(s <= n && r >= n - s && solution[i] <= n - s) {
        if(n == s) {
            for(int j = 0; j <= i; ++j) {
                if(x[j] == 1) {
                    solution_final.push_back(solution[j]);
                }
            }
        } else {
            solution_helper(i + 1, s, r - solution[i], n);
        }
    }
}
