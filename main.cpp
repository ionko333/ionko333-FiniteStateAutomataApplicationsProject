#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <fstream>
#include <limits>
#include <type_traits>

using namespace std;

template<typename T>
void hash_combine(std::size_t& seed, const T& data) ///from boost     https://www.boost.org/doc/libs/1_35_0/doc/html/boost/hash_combine_id241013.html
{
	static std::hash<T> hasher;
	seed ^= hasher(data) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std
{
	template<typename T1, typename T2>        ///hashcode for pair
	struct hash<std::pair<T1, T2>>
	{
		std::size_t operator()(const pair<T1, T2>& p)const
		{
			std::size_t seed = 0;
			hash_combine(seed, p.first);
			hash_combine(seed, p.second);
			return seed;
		}
	};
}

template <typename T>
set<T> setUnion(const set<T>& a, const set<T>& b)///returns the union of two sets
{
	set<T> res = a;
	for (auto it = b.begin(); it != b.end(); it++)
	{
		res.insert(*it);
	}
	return res;
}

template <typename T>
set<T> interSection(const set<T>& a, const set<T>& b)///returns the intersection of two sets
{
	set<T> result;
	for (auto it = a.begin(); it != a.end(); it++)
	{
		if (b.find(*it) != b.end())
		{
			result.insert(*it);
		}
	}
	for (auto it = b.begin(); it != b.end(); it++)
	{
		if (a.find(*it) != a.end())
		{
			result.insert(result.begin(), *it);
		}
	}
	return result;
}

template <typename T>
bool isSubset(const set<T>&a, const set<T>& b)
{
	for (auto it = a.begin(); it != a.end(); it++)
	{
		if (b.find(*it) == b.end())
		{
			return false;
		}
	}
	return true;
}

set<pair<int, int> > Compose(const set<pair<int, int> >& rel1, const set<pair<int, int> >& rel2)///composes two relations
{
	set<pair<int, int> > temp2;
	for (auto it = rel1.begin(); it != rel1.end(); it++)
	{
		for (auto it2 = rel2.begin(); it2 != rel2.end(); it2++)
		{
			if (it->second == it2->first)
			{
				temp2.insert({ it->first, it2->second });
			}
		}
	}
	return temp2;
}

set<pair<int, int> > transClosure(set<pair<int, int> >& rel) ///returns the transitive closure of a relation
{
    cout << "Transitive closure on " << rel.size() << " states \n";
	set<pair<int, int> > C;
	set<pair<int, int> > D = rel; ///R
	set<pair<int, int> > R = D;
	while (!isSubset(D, C))
	{
		C = setUnion(C, R);  ///union of R^1 to R^i
		R = Compose(R, rel); ///R^_{i+1}
		D = setUnion(C, R);  ///union of R^1 to R^{i + 1}
	}
	return C;
}

template <typename T>
int posOfEl(const T& el, const vector<T>& v)
{
	int cnt = 0;
	for (auto it = v.begin(); it != v.end(); it++)
	{
		if (*it == el)
		{
			return cnt;
		}
		cnt++;
	}
	return -1;
}

template <typename T>
T getLast(const set<T> s)
{
	auto it = s.end();
	it--;
	return *it;
}

string sigma(int n, const string& w)
{
	if (n < w.size())
	{
		return string(1, w[n]);
	}
	return "";
}

pair<int, int> orderMinMax(int a, int b)
{
	if (a >= b)
	{
		return { b,a };
	}
	return { a,b };
}

template <typename T>
void printSet(const set<T>& s)
{
	for (auto it = s.begin(); it != s.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

template <typename T>
vector<T> setToVec(const set<T>& s)
{
	return vector<T>(s.begin(), s.end());
}

template <typename T>
set<T> vecToSet(const vector<T>& v)
{
	set<T> res(v.begin(), v.end());
	return res;
}

template <typename T>
struct Transition
{
	T word;
	int start;
	int finish;
	Transition<T>(int s = 0, const T& w = T(), int f = 0) : word(w), start(s), finish(f) {}
	bool operator<(const Transition<T>& other)const
	{
		if (start < other.start)
		{
			return true;
		}
		else if (start == other.start)
		{
			if (finish < other.finish)
			{
				return true;
			}
			else if (finish == other.finish)
			{
				return word < other.word;
			}
		}
		return false;
	}
	void printTr()const;
	bool isEmpty()const;
};

template<>
void Transition<string>::printTr()const
{
	cout << start << " :" << word << ": " << finish << endl;
}

template<>
void Transition<pair<string, string> >::printTr()const
{
	cout << start << " :" << word.first << ": :" << word.second << ": " << finish << endl;
}

template<typename T>
void printTr(const Transition<T>& tr)
{
	tr.printTr();
}

template<>
bool Transition<string>::isEmpty()const
{
	string str;
	bool emp = word == str;
	return emp;
}

template<>
bool Transition<pair<string, string> >::isEmpty()const
{
	bool eq = word == pair<string, string>();
	return eq;
}

template <typename T>
struct FSA
{
	int states;
	set<int> startingStates;
	set<int> finalStates;
	set<Transition<T> > delta;
	FSA(int s = 0, const set<int>& st = set<int>(), const set<int>& fin = set<int>(), const set<Transition<T> >& d = set<Transition<T> >())
		:states(s), startingStates(st), finalStates(fin), delta(d)
	{
	}
	FSA(const string& w);
	FSA(const pair<string, string>& w);
	void expandFSA();

};

template<>
void FSA<string>::expandFSA()
{
	set<Transition<string> > toProcess;
	set<Transition<string> > newDelta;
	for (auto it = delta.begin(); it != delta.end(); it++)
	{
		if (it->word.length() > 1)
		{
			toProcess.insert(*it);
		}
		else
		{
			newDelta.insert(*it);
		}
	}
	int currMax = states;
	for (auto it = toProcess.begin(); it != toProcess.end(); it++)
	{
		int len = it->word.length();
		vector<int> newStates;
		newDelta.insert(Transition<string>(it->start, string(1, it->word[0]), currMax + 1));
		for (int i = 1; i < len - 1; i++)
		{
			newDelta.insert(Transition<string>(currMax + i, string(1, it->word[i]), currMax + i + 1));
		}
		newDelta.insert(Transition<string>(currMax + len - 1, string(1, it->word[len - 1]), it->finish));
		currMax += len - 1;
	}

	*this = FSA<string>(currMax, startingStates, finalStates, newDelta);
}

template<>
void FSA<pair<string, string> >::expandFSA()
{
	set<Transition<pair<string, string> > > toProcess;
	set<Transition<pair<string, string> > > newDelta;
	for (auto it = delta.begin(); it != delta.end(); it++)
	{
		if (it->word.first.length() > 1 || it->word.second.length() > 1)
		{
			toProcess.insert(*it);
		}
		else
		{
			newDelta.insert(*it);
		}
	}
	int currMax = states;
	for (auto it = toProcess.begin(); it != toProcess.end(); it++)
	{
		int len1 = it->word.first.length();
		int len2 = it->word.second.length();
		auto minMax = orderMinMax(len1, len2);
		vector<int> newStates;
		newDelta.insert(Transition<pair<string, string> >(it->start, { sigma(0,it->word.first),sigma(0,it->word.second) }, currMax + 1));
		for (int i = 1; i < minMax.second - 1; i++)
		{
			newDelta.insert(Transition<pair<string, string> >(currMax + i, { sigma(i,it->word.first),sigma(i,it->word.second) }, currMax + i + 1));
		}
		newDelta.insert(Transition<pair<string, string> >
			(currMax + minMax.second - 1, { sigma(minMax.second - 1,it->word.first), sigma(minMax.second - 1,it->word.second) }, it->finish));
		currMax += minMax.second - 1;
	}

	*this = FSA<pair<string, string> >(currMax, startingStates, finalStates, newDelta);
}

template<>
FSA<string>::FSA(const string& w)
{
	set<Transition<string> > tr;
	tr.insert(Transition<string>(1, w, 2));
	set<int> st;
	st.insert(1);
	set<int> fins;
	fins.insert(2);

	*this = FSA<string>(2, st, fins, tr);
}

template<>
FSA<pair<string, string> >::FSA(const pair<string, string>& w)
{
	set<Transition<pair<string, string> > > tr;
	tr.insert(Transition<pair<string, string> >(1, w, 2));
	set<int> st;
	st.insert(1);
	set<int> fins;
	fins.insert(2);

	*this = FSA<pair<string, string> >(2, st, fins, tr);
}

template <typename T>
void printFSA(const FSA<T>& a)
{
	cout << "States are: 1 to " << a.states << endl;
	cout << "Starting states are : ";
	printSet(a.startingStates);
	cout << "Final states are: ";
	printSet(a.finalStates);
	cout << "Transitions are: " << endl;
	for (auto it = a.delta.begin(); it != a.delta.end(); it++)
	{
		it->printTr();
	}
}

set<int> remapStates(int k, const set<int>& st)
{
	set<int> newSt;
	for (auto it = st.begin(); it != st.end(); it++)
	{
		int a = *it;
		newSt.insert(a + k);
	}
	return newSt;
}

template <typename T>
set<Transition<T> > remapFSAdelta(int k, const set<Transition<T> >& a)
{
	set<Transition<T> > newT;
	for (auto it = a.begin(); it != a.end(); it++)
	{
		int s = it->start + k;
		int f = it->finish + k;
		newT.insert(Transition<T>(s, it->word, f));
	}

	return newT;
}

template <typename T>
FSA<T> remapFSA(int s, const FSA<T>& a)
{
	int states = a.states + s;
	set<int> newStart = remapStates(s, a.startingStates);
	set<int> newFinal = remapStates(s, a.finalStates);
	set<Transition<T> > newDelta = remapFSAdelta(s, a.delta);

	return FSA<T>(states, newStart, newFinal, newDelta);
}

template<typename T>
FSA<T> unionFSA(const FSA<T>& a, const FSA<T>& b)
{
	FSA<T> remappedB = remapFSA<T>(a.states, b);
	int st = remappedB.states;
	set<int> beg = setUnion(a.startingStates, remappedB.startingStates);
	set<int> fin = setUnion(a.finalStates, remappedB.finalStates);
	set<Transition<T> > delt = setUnion(a.delta, remappedB.delta);

	return FSA<T>(st, beg, fin, delt);
}

template<typename T>
FSA<T> concatFSA(const FSA<T>& a, const FSA<T>& b)
{
	FSA<T> remappedB = remapFSA(a.states, b);
	int st = remappedB.states;
	set<Transition<T> > extra = setUnion(a.delta, remappedB.delta);
	for (auto it = a.finalStates.begin(); it != a.finalStates.end(); it++)
	{
		for (auto it2 = remappedB.startingStates.begin(); it2 != remappedB.startingStates.end(); it2++)
		{
			extra.insert(Transition<T>(*it, T(), *it2));
		}
	}

	return FSA<T>(st, a.startingStates, remappedB.finalStates, extra);
}

template<typename T>
FSA<T> starFSA(const FSA<T>& a)
{
	int q = 1;
	FSA<T> remappedA = remapFSA<T>(q, a);
	int states = remappedA.states;
	remappedA.finalStates.insert(q);
	set<int> st;
	st.insert(q);
	set<Transition<T> > deltaPrime = remappedA.delta;
	for (auto it = remappedA.startingStates.begin(); it != remappedA.startingStates.end(); it++)
	{
		deltaPrime.insert(Transition<T>(q, T(), *it));
	}
	for (auto it = remappedA.finalStates.begin(); it != remappedA.finalStates.end(); it++)
	{
		deltaPrime.insert(Transition<T>(*it, T(), q));
	}

	return FSA<T>(states, st, remappedA.finalStates, deltaPrime);
}

template<typename T>
FSA<T> plusFSA(const FSA<T>& a)
{
	int q = 1;
	FSA<T> remappedA = remapFSA<T>(q, a);
	set<int> st;
	st.insert(q);
	int states = remappedA.states;
	set<Transition<T> > extra = remappedA.delta;
	for (auto it = remappedA.startingStates.begin(); it != remappedA.startingStates.end(); it++)
	{
		extra.insert(Transition<T>(q, T(), *it));
	}
	for (auto it = remappedA.finalStates.begin(); it != remappedA.finalStates.end(); it++)
	{
		extra.insert(Transition<T>(*it, T(), q));
	}
	return FSA<T>(states, st, remappedA.finalStates, extra);
}

template<typename T>
FSA<T> forAllPlus(const set<T>& s)
{
	set<Transition<T> > delt;
	for (auto it = s.begin(); it != s.end(); it++)
	{
		delt.insert(Transition<T>(1, *it, 1));
		delt.insert(Transition<T>(1, *it, 2));
	}
	set<int> fins;
	fins.insert(2);
	set<int> st;
	st.insert(1);

	return FSA<T>(2, st, fins, delt);
}

template<typename T>
FSA<T> forAllStar(const set<T>& s)
{
	set<Transition<T> > delt;
	for (auto it = s.begin(); it != s.end(); it++)
	{
		delt.insert(Transition<T>(1, *it, 1));
	}
	set<int> fins;
	fins.insert(1);
	set<int> st;
	st.insert(1);

	return FSA<T>(1, st, fins, delt);
}

template<typename T>
set<pair<int, int> > projOneThree(const set<Transition<T> >& delta)
{
	set<pair<int, int> > res;
	for (auto it = delta.begin(); it != delta.end(); it++)
	{
		res.insert({ it->start,it->finish });
	}

	return res;
}

template <typename T1, typename T2>
unordered_map<T1, vector<T2> > funcOneTwo(const set<pair<T1, T2> >& C)
{
	unordered_map<T1, vector<T2> > func;
	for (auto it = C.begin(); it != C.end(); it++)
	{
		if (func.find(it->first) == func.end())
		{
			vector<T2> v;
			v.push_back(it->second);
			func.insert({ it->first,v });
		}
		else
		{
			func[it->first].push_back(it->second);
		}
	}

	return func;
}

template <typename T>
unordered_map<int, vector<pair<T, int> > > funcOneTwoThree(const set<Transition<T> >& tr)
{
	unordered_map<int, vector<pair<T, int> > > func;

	for (auto it = tr.begin(); it != tr.end(); it++)
	{
		if (func.find(it->start) == func.end())
		{
			vector<pair<T, int> > v;
			v.push_back({ it->word,it->finish });
			func.insert({ it->start,v });
		}
		else
		{
			func[it->start].push_back({ it->word,it->finish });
		}
	}

	return func;
}

template<typename T>
set<Transition<T> > getReverseDelta(const set<Transition<T> >& tr)
{
	set<Transition<T> > rev;
	for (auto it = tr.begin(); it != tr.end(); it++)
	{
		rev.insert(Transition<T>(it->finish, it->word, it->start));
	}

	return rev;
}

template<typename T>
unordered_map<int, vector<int> > getAdj(const set<Transition<T> >& tr)
{
	unordered_map<int, vector<int> > adj;
	for (auto it = tr.begin(); it != tr.end(); it++)
	{
		if (adj.find(it->start) == adj.end())
		{
			vector<int> v;
			v.push_back(it->finish);
			adj.insert({ it->start,v });
		}
		else
		{
			adj[it->start].push_back(it->finish);
		}
	}

	return adj;
}

template<typename T>
vector<bool> getReachable(const FSA<T>& a, bool isReachable)
{
	vector<bool> visited;
	visited.resize(a.states + 1);
	fill(visited.begin(), visited.end(), 0);
	queue<int> q;
	unordered_map<int, vector<int> > adj;
	if (isReachable)
	{
		for (auto it = a.startingStates.begin(); it != a.startingStates.end(); it++)
		{
			visited[*it] = true;
			q.push(*it);
		}
		adj = getAdj(a.delta);
	}
	else
	{
		for (auto it = a.finalStates.begin(); it != a.finalStates.end(); it++)
		{
			visited[*it] = true;
			q.push(*it);
		}
		adj = getAdj(getReverseDelta(a.delta));
	}
	while (!q.empty())
	{
		int v = q.front();
		q.pop();
		for (auto it = adj[v].begin(); it != adj[v].end(); it++)
		{
			if (visited[*it])
			{
				continue;
			}
			visited[*it] = true;
			q.push(*it);
		}
	}

	return visited;
}

template<typename T>
FSA<T> removeEpsilon(const FSA<T>& a)
{
	set<pair<int, int> > toProcess;
	for (auto it = a.delta.begin(); it != a.delta.end(); it++)
	{
		if (it->isEmpty())
		{
			toProcess.insert({ it->start,it->finish });
		}
	}
	set<pair<int, int> > R = transClosure(toProcess);
	for (int i = 1; i <= a.states; i++)
	{
		R.insert({ i,i });
	}
	unordered_map<int, vector<int> > func = funcOneTwo(R);
	set<Transition<T> > deltaPrime;
	for (auto it = a.delta.begin(); it != a.delta.end(); it++)
	{
		if (!it->isEmpty())
		{
			for (auto it2 = func[it->finish].begin(); it2 != func[it->finish].end(); it2++) ///(q1,a,q') in delta and q2 in Ce(q') => (q1,a,a2) in delta Prime
			{
				deltaPrime.insert(Transition<T>(it->start, it->word, *it2));
			}
		}
	}
	set<int> startingStates;
	for (auto it = a.startingStates.begin(); it != a.startingStates.end(); it++)  ///Ce(I)
	{
	    for(auto it2 = func[*it].begin(); it2 != func[*it].end(); it2++)
        {
            startingStates.insert(*it2);
        }
	}

	return FSA<T>(a.states, startingStates, a.finalStates, deltaPrime);
}

template<typename T>
FSA<T> trimFSA(const FSA<T>& a)
{
	vector<bool> reachable = getReachable(a, true);
	vector<bool> coReachable = getReachable(a, false);
	vector<bool> newStates;
	fill(newStates.begin(), newStates.end(), 0);
	newStates.resize(a.states + 1);
	unordered_map<int, int> mapping;
	for (int i = 1; i <= a.states; i++)
	{
		if (reachable[i] && coReachable[i])
		{
			newStates[i] = true;
		}
	}
	int j = 1;
	for (int i = 1; i <= a.states; i++)
	{
		if (newStates[i])
		{
			mapping.insert({ i,j });
			j++;
		}
	}
	set<int> st;

	for (auto it = a.startingStates.begin(); it != a.startingStates.end(); it++)
	{
		if (newStates[*it])
		{
			st.insert(mapping[*it]);
		}
	}

	set<int> fins;
	for (auto it = a.finalStates.begin(); it != a.finalStates.end(); it++)
	{
		if (newStates[*it])
		{
			fins.insert(mapping[*it]);
		}
	}

	set<Transition<T> > newDelta;
	for (auto it = a.delta.begin(); it != a.delta.end(); it++)
	{
		if (newStates[it->start] && newStates[it->finish])
		{
			newDelta.insert(Transition<T>(mapping[it->start], it->word, mapping[it->finish]));
		}
	}

	return FSA<T>(j - 1, st, fins, newDelta);
}

template<typename T>
FSA<T> trimDFSA(const FSA<T>& a)
{
	if (a.states == 0)
	{
		set<int> st;
		st.insert(1);
		return FSA<T>(1, st, set<int>(), set<Transition<T> >());
	}

	return trimFSA(a);
}

template<typename T>
FSA<T> determinate(const FSA<T>& a)
{
	FSA<T> b = removeEpsilon(a);
	b.expandFSA();
	unordered_map<int, vector<pair<T, int> > > func = funcOneTwoThree(b.delta);
	int i = 0;
	vector<set<int> > P;
	P.push_back(b.startingStates);
	set<Transition<T> > delta;
	while (i != P.size())
	{
		set<pair<T, int> > N;  /// N is union of deltaPrime(P[i])
		for (auto it = P[i].begin(); it != P[i].end(); it++)
		{
		    for(auto it2 = func[*it].begin(); it2 != func[*it].end(); it2++)
            {
                N.insert(*it2);
            }
		}
		unordered_map<T, set<int> > nPrime; /// nPrime is funcOneTwo(N)
		for(auto it = N.begin(); it != N.end(); it++)
        {
            if(nPrime.find(it->first) == nPrime.end())
            {
                set<int> s;
                s.insert(it->second);
                nPrime.insert({it->first,s});
            }
            else
            {
                nPrime[it->first].insert(it->second);
            }
        }

		for (auto it = nPrime.begin(); it != nPrime.end(); it++)
		{
			if (posOfEl(it->second, P) == -1)
			{
				P.push_back(it->second);
			}
		}
		for (auto it = nPrime.begin(); it != nPrime.end(); it++)
		{
			int pos = posOfEl(it->second, P);
			delta.insert(Transition<T>(i + 1, it->first, pos + 1));
		}
		i++;
	}
	set<int> starting;
	starting.insert(1);
	set<int> newFinal;
	for (int i = 1; i <= P.size(); i++)
	{
		if (!interSection(P[i - 1], b.finalStates).empty()) ///if it contains a final -> it is now final
		{
			newFinal.insert(i);
		}
	}

	return trimDFSA(FSA<T>(P.size(), starting, newFinal, delta));
}

pair<vector<pair<int, int> >, set<Transition<string> > >  product(int s1, const set<Transition<string> >& d1, int s2, const set<Transition<string> >& d2)
{
	unordered_map<int, vector<pair<string, int> > > dPrime = funcOneTwoThree(d1);
	vector<pair<int, int> > P;
	P.push_back({ s1,s2 });
	set<Transition<string> > delt;
	int i = 0;
	while (i != P.size())
	{
		pair<int, int> ps = P[i]; /// <p1,p2> = P[i]
		unordered_set<pair<string, pair<int, int>> > N; /// N = {(c,(q1,delta2(p2,c)))|(c,q1) in delta1Prime(p1) & !delta2(p2,c)}
		for (auto it = dPrime[ps.first].begin(); it != dPrime[ps.first].end(); it++)
		{
			for (auto it2 = d2.begin(); it2 != d2.end(); it2++)
			{
				if (it2->word == it->first && it2->start == ps.second)
				{
					N.insert({ it2->word,{it->second,it2->finish} });
				}
			}
		}
		for (auto it = N.begin(); it != N.end(); it++)
		{
			if (posOfEl(it->second, P) == -1)
			{
				P.push_back(it->second);
			}
		}
		for (auto it = N.begin(); it != N.end(); it++)
		{
			delt.insert(Transition<string>(i + 1, it->first, posOfEl(it->second, P) + 1));
		}
		i++;
	}
	return { P,delt };
}

FSA<string> intersectDFSA(const FSA<string>& a, const FSA<string>& b)
{
	pair<vector<pair<int, int> >, set<Transition<string> > > res = product(*a.startingStates.begin(), a.delta, *b.startingStates.begin(), b.delta);
	vector<pair<int, int> > P = res.first;
	set<int> fins;
	for (int i = 0; i < P.size(); i++)
	{
		if (a.finalStates.find(P[i].first) != a.finalStates.end() && b.finalStates.find(P[i].second) != b.finalStates.end())
		{
			fins.insert(i + 1);
		}
	}
	set<int> st;
	st.insert(1);

	FSA<string> c(P.size(), st, fins, res.second);

	return c;

}

FSA<string> intersectFSA(const FSA<string>& a, const FSA<string>& b)
{
	FSA<string> c = determinate(a);
	FSA<string> d = determinate(b);
	FSA<string> e = intersectDFSA(c, d);

	return e;
}

FSA<string> diffDFSA(const FSA<string>& a, const FSA<string>& b)
{
	set<Transition<string> > del2;
	unordered_set<pair<int, string> > used;
	set<string> chars;
	for (auto it = b.delta.begin(); it != b.delta.end(); it++)
	{
		chars.insert(it->word);
	}
	int maxState = b.states;
	for (auto it = b.delta.begin(); it != b.delta.end(); it++)
	{
		del2.insert(*it);
		used.insert({ it->start,it->word });
	}
	for (auto it = a.delta.begin(); it != a.delta.end(); it++)
	{
		chars.insert(it->word);
	}
	for (int i = 0; i <= maxState; i++)
	{
		for (auto it = chars.begin(); it != chars.end(); it++)
		{
			if (used.find({ i,*it }) == used.end())
			{
				del2.insert(Transition<string>(i, *it, 0));
			}
		}
	}
	auto p = product(*a.startingStates.begin(), a.delta, *b.startingStates.begin(), del2);
	vector<pair<int, int> > P = p.first;
	set<Transition<string> > d = p.second;
	set<int> fins;
	for (int i = 0; i < P.size(); i++)
	{
		if (a.finalStates.find(P[i].first) != a.finalStates.end() && b.finalStates.find(P[i].second) == b.finalStates.end())
		{
			fins.insert(i + 1);
		}
	}
	set<int> st;
	st.insert(1);
	FSA<string> t(P.size(), st, fins, d);

	return trimDFSA(t);
}

FSA<string> diffFSA(const FSA<string>& a, const FSA<string>& b)
{
	FSA<string> c = determinate(a);
	FSA<string> d = determinate(b);
	FSA<string> e = diffDFSA(c, d);

	return e;
}

void testIntersection()
{
	FSA<string> a("abcd");
	FSA<string> b("woah");
	FSA<string> c("pog");
	FSA<string> d("fml");
	FSA<string> first = unionFSA(unionFSA(a, b), unionFSA(c, d));
	cout << "Printing first : \n";
	printFSA(first);
	FSA<string> e("zxyz");
	FSA<string> f("pog");
	FSA<string> g("fml");
	FSA<string> h("nope");
	FSA<string> second(unionFSA(unionFSA(e, f), unionFSA(g, h)));
	cout << "Printing second : \n";
	printFSA(second);
	FSA<string> fin = diffFSA(first, second);
	printFSA(fin);
}

FSA<string> domainFST(const FSA<pair<string, string> >& a)
{
	set<Transition<string> > delt;
	for (auto it = a.delta.begin(); it != a.delta.end(); it++)
	{
		delt.insert(Transition<string>(it->start, it->word.first, it->finish));
	}

	return FSA<string>(a.states, a.startingStates, a.finalStates, delt);
}

FSA<string> rangeFST(const FSA<pair<string, string> > & a)
{
	set<Transition<string> > delt;
	for (auto it = a.delta.begin(); it != a.delta.end(); it++)
	{
		delt.insert(Transition<string>(it->start, it->word.second, it->finish));
	}

	return FSA<string>(a.states, a.startingStates, a.finalStates, delt);
}

FSA<pair<string, string> > identityFSA(const FSA<string>& a)
{
	set<Transition<pair<string, string> > > delt;
	for (auto it = a.delta.begin(); it != a.delta.end(); it++)
	{
		delt.insert(Transition<pair<string, string> >(it->start, { it->word,it->word }, it->finish));
	}
	return FSA<pair<string, string> >(a.states, a.startingStates, a.finalStates, delt);
}

FSA<pair<string, string> > composeFST(const FSA<pair<string, string> >& a, const FSA<pair<string, string> >& b)
{
	FSA<pair<string, string> > c = a;
	FSA<pair<string, string> > d = b;
	c.expandFSA();
	d.expandFSA();

	set<Transition<pair<string, string> > > Edelt1 = c.delta;
	set<Transition<pair<string, string> > > Edelt2 = d.delta;

	for (int i = 1; i <= c.states; i++)
	{
		Edelt1.insert(Transition<pair<string, string> >(i, make_pair("", ""), i));
	}
	for (int i = 1; i <= d.states; i++)
	{
		Edelt2.insert(Transition<pair<string, string> >(i, make_pair("", ""), i));
	}

	unordered_map<int, vector<pair< pair<string, string>, int> > > delt1 = funcOneTwoThree(Edelt1);
	unordered_map<int, vector<pair< pair<string, string>, int> > > delt2 = funcOneTwoThree(Edelt2);
	vector<pair<int, int> > P;
	for (auto it = c.startingStates.begin(); it != c.startingStates.end(); it++)
	{
		for (auto it2 = d.startingStates.begin(); it2 != d.startingStates.end(); it2++)
		{
			P.push_back({ *it,*it2 });
		}
	}
	set<Transition<pair<string, string> > > newDelta;
	int i = 0;
	while (i != P.size())
	{
		pair<int, int> ps = P[i];
		vector<pair<pair<string, string>, int> > D1 = delt1[ps.first];
		vector<pair<pair<string, string>, int> > D2 = delt2[ps.second];
		vector<pair<pair<string, string>, pair<int, int> > > N;
		for (auto it = D1.begin(); it != D1.end(); it++)
		{
			for (auto it2 = D2.begin(); it2 != D2.end(); it2++)
			{
				if (it->first.second == it2->first.first)
				{
					N.push_back({ {it->first.first,it2->first.second},{it->second,it2->second} });
				}
			}
		}
		for (auto it = N.begin(); it != N.end(); it++) ///N = {((a1,b2),(q1,q2))|((a1,c),q1) in D1, ((c,b2),q2) in D2
		{
			if (posOfEl(it->second, P) == -1)
			{
				P.push_back(it->second);
			}
		}
		for (auto it = N.begin(); it != N.end(); it++)
		{
			int pos = posOfEl(it->second, P);
			newDelta.insert(Transition<pair<string, string> >(i + 1, it->first, pos + 1));
		}
		i++;
	}
	set<int> newStarting;
	set<int> newFinals;
	for (int i = 1; i <= P.size(); i++)
	{
		if (a.finalStates.find(P[i - 1].first) != a.finalStates.end() && b.finalStates.find(P[i - 1].second) != b.finalStates.end())
		{
			newFinals.insert(i);
		}
		if (a.startingStates.find(P[i - 1].first) != a.startingStates.end() && b.startingStates.find(P[i - 1].second) != b.startingStates.end())
		{
			newStarting.insert(i);
		}
	}

	FSA<pair<string, string> > gPrime = FSA<pair<string, string> >(P.size(), newStarting, newFinals, newDelta);

	return trimFSA(removeEpsilon(gPrime));
}

void testCompose()
{
	FSA<pair<string, string> > a(make_pair("abcd", "woah"));
	FSA<pair<string, string> > b(make_pair("woah", "yonko"));
	FSA<pair<string, string> > c = composeFST(a, b);
	printFSA(c);
}

pair<vector<pair<pair<int, int>, string> >, bool > CEfst(const set<pair<pair<int, int>, string> >& A) /// similar to transitive closure for epsilon transitions, check for eps loops
{
	unordered_map<int, vector<pair<int, string> > > aPrime;
	for (auto it = A.begin(); it != A.end(); it++)
	{
		if (aPrime.find(it->first.first) == aPrime.end())
		{
			vector<pair<int, string> > s;
			s.push_back({ it->first.second, it->second });
			aPrime.insert({ it->first.first,s });
		}
		else
		{
			aPrime[it->first.first].push_back({ it->first.second,it->second });
		}
	}
	vector<pair<pair<int, int>, string> > T = setToVec(A);
	bool inf = false;
	int i = 0;
	while (i != T.size() && !inf)
	{
		pair<pair<int, int>, string> curr = T[i];
		inf = (curr.first.first == curr.first.second) && curr.second != "";   ///means <e,u> cycle for u != e -> infinitely ambigious
		for (auto it = aPrime[curr.first.second].begin(); it != aPrime[curr.first.second].end(); it++)
		{
			T.push_back(make_pair(make_pair(curr.first.first, it->first), curr.second + it->second));
		}
		i++;
	}

	///cout << "Value of inf before returning is : " << inf << endl;

	return make_pair(T, inf);
}

FSA<pair<string, string> > removeUpperEpsilon(const FSA<pair<string, string> >& a, set<string>& W, bool& inf)
{
	set<pair<pair<int, int>, string> > tr;
	for (auto it = a.delta.begin(); it != a.delta.end(); it++)
	{
		if (it->word.first == "")
		{
			tr.insert({ {it->start,it->finish},it->word.second });
		}
	}
	auto res = CEfst(tr);
	vector<pair<pair<int, int>, string> > C = res.first;
	for (int i = 1; i <= a.states; i++)
	{
		C.push_back({ {i,i},"" });
	}
	set<int> fins = a.finalStates;
	for (auto it = C.begin(); it != C.end(); it++)
	{
		if (a.startingStates.find(it->first.first) != a.startingStates.end() && a.finalStates.find(it->first.second) != a.finalStates.end())
		{
			fins.insert(it->first.first);
			W.insert(it->second);         ///number of <e,u> words if we have more than 1 -> T is not functional
		}
	}
	unordered_map<int, vector<pair<int, string> > > Ce;
	unordered_map<int, vector< pair<int, string> > > CePrime;
	for (auto it = C.begin(); it != C.end(); it++)
	{
		if (Ce.find(it->first.first) == Ce.end())
		{
			vector<pair<int, string> > s;
			s.push_back({ it->first.second, it->second });
			Ce.insert({ it->first.first,s });
		}
		else
		{
			Ce[it->first.first].push_back({ it->first.second,it->second });
		}
	}
	for (auto it = C.begin(); it != C.end(); it++)
	{
		if (CePrime.find(it->first.second) == CePrime.end())
		{
			vector<pair<int, string> > s;
			s.push_back({ it->first.first, it->second });
			CePrime.insert({ it->first.second,s });
		}
		else
		{
			CePrime[it->first.second].push_back({ it->first.first,it->second });
		}
	}

	set<Transition<pair<string, string> > > newDelta;
	for (auto it = a.delta.begin(); it != a.delta.end(); it++)
	{
		if (it->word.first != "")
		{
			for (auto it2 = Ce[it->finish].begin(); it2 != Ce[it->finish].end(); it2++)
			{
				for (auto it3 = CePrime[it->start].begin(); it3 != CePrime[it->start].end(); it3++)
				{
					newDelta.insert(Transition<pair<string, string> >(it3->first, { it->word.first,it3->second + it->word.second + it2->second }, it2->first));
				}
			}
		}
	}

	inf = res.second;


	return FSA<pair<string, string> >(a.states, a.startingStates, fins, newDelta);
}

FSA<pair<string, string> > realTime(const FSA<pair<string, string> >& a, set<string>& W, bool& inf)
{
	return removeUpperEpsilon(a, W, inf);
}

FSA<pair<string, string> > squaredOutputTransducer(const FSA<pair<string, string> >& a)
{
	unordered_map<int, vector<pair<pair <string, string>, int> > > func = funcOneTwoThree(a.delta);
	vector<pair<int, int> > P;
	set<Transition<pair<string, string> > > deltaPrime;
	set<int> st2 = a.startingStates;
	for (auto it = a.startingStates.begin(); it != a.startingStates.end(); it++) ///I x I
	{
		for (auto it2 = st2.begin(); it2 != st2.end(); it2++)
		{
			P.push_back({ *it,*it2 });
		}
	}
	int i = 0;
	while (i != P.size())
	{
		pair<int, int> ps = P[i];
		vector<pair<pair<string, string>, int> > D1;
		vector<pair<pair<string, string>, int> > D2;
		if (func.find(ps.first) != func.end())
		{
			D1 = func[ps.first];
		}
		if (func.find(ps.second) != func.end())
		{
			D2 = func[ps.second];
		}
		vector<pair<pair<string, string>, pair<int, int> > > N; ///((a,alpha1),q1) in D1 ,((a,alpha2),q2) in D2 => ((alpha1,alpha2),(q1,q2)) in N
		for (auto it = D1.begin(); it != D1.end(); it++)
		{
			for (auto it2 = D2.begin(); it2 != D2.end(); it2++)
			{
				if (it->first.first == it2->first.first)
				{
					N.push_back({ {it->first.second,it2->first.second},{it->second,it2->second} });
				}
			}
		}
		for (auto it = N.begin(); it != N.end(); it++)
		{
			if (posOfEl(it->second, P) == -1)
			{
				P.push_back(it->second);
			}
		}
		for (auto it = N.begin(); it != N.end(); it++)
		{
			deltaPrime.insert(Transition<pair<string, string> >(i + 1, { it->first.first,it->first.second }, posOfEl(it->second, P) + 1));
		}
		i++;
	}
	set<int> newStarting;
	set<int> newFinal;
	for (int i = 0; i < P.size(); i++)
	{
		if (a.finalStates.find(P[i].first) != a.finalStates.end() && a.finalStates.find(P[i].second) != a.finalStates.end())
		{
			newFinal.insert(i + 1);
		}
		if (a.startingStates.find(P[i].first) != a.startingStates.end() && a.startingStates.find(P[i].second) != a.startingStates.end())
		{
			newStarting.insert(i + 1);
		}
	}

	return FSA<pair<string, string> >(P.size(), newStarting, newFinal, deltaPrime);
}

void squareOutputTest()
{
	FSA<pair<string, string> > a(make_pair("woah", "pog"));
	FSA<pair<string, string> > b(make_pair("gosho", "losho"));
	FSA<pair<string, string> > c = unionFSA<pair<string, string> >(a, b);
	FSA<pair<string, string> > d = squaredOutputTransducer(c);
	printFSA<pair<string, string> >(d);
}

bool isBalancible(const string& u, const string& v)
{
	return (u == "") || (v == "");
}

string commonPrefix(const string& u, const string& v)
{
	string str;
	int len = min(u.length(), v.length());
	str.reserve(len);
	for (int i = 0; i < len; i++)
	{
		if (u[i] == v[i])
		{
			str += u[i];
		}
		else
		{
			break;
		}
	}
	return str;
}

string remainderSuffix(const string& u, const string& v)
{
	int len1 = u.length();
	int len2 = v.length();
	return v.substr(len1, len2 - len1);
}

pair<string, string> advanceFunc(const pair<string, string>& u, const pair<string, string>& v)
{
	const string fw = u.first + v.first;
	const string sw = u.second + v.second;
	const string c = commonPrefix(fw, sw);
	const string a = remainderSuffix(c, fw);
	const string b = remainderSuffix(c, sw);
	return { a, b };
}

bool testFunctionality(const FSA<pair<string, string> >& a)
{
	bool inf = false;
	set<string> W;
	FSA<pair<string, string> > rt = realTime(a, W, inf);
	if(inf)
    {
        cout << "Transducer is infinitely ambigious! \n";

        return false;
    }

    if(W.size() > 1)
    {
        cout << "We have more than one pair with the empty on the left! \n";

        return false;
    }
	FSA<pair<string, string> > b = trimFSA(squaredOutputTransducer(rt));
	unordered_map<int, vector<pair<pair<string, string>, int> > > func = funcOneTwoThree(b.delta);
	vector<pair<int, pair<string, string> > > Adm;
	for (auto it = b.startingStates.begin(); it != b.startingStates.end(); it++)
	{
		Adm.push_back({ *it,{"",""} });
	}
	int i = 0;
	while (i != Adm.size())
	{
		pair<int, pair<string, string> > ps = Adm[i];
		vector<pair<int, pair<string, string> > > Dq;
		for (auto it = func[ps.first].begin(); it != func[ps.first].end(); it++)
		{
			Dq.push_back({ it->second,{advanceFunc(ps.second,it->first)} });
		}
		for (auto it = Dq.begin(); it != Dq.end(); it++)
		{
			if (!isBalancible(it->second.first, it->second.second)) ///if it is unbalancible then T is not functional
			{
				cout << "We have reached an unbalancible admissible advance! \n";

				return false;
			}
			pair<string, string> str = { "","" };
			if (b.finalStates.find(it->first) != b.finalStates.end() && it->second != str)   ///if we are a final State and we have a pair != <e,e>
			{
				cout << "We have reached a final state with nonempty label! \n";

				return false;
			}
			for (int j = 0; j < Adm.size(); j++)                                             ///exists a state with more than 1 advance -> false
			{
				if (Adm[j].first == it->first && Adm[j].second != it->second)
				{
					cout << "We have reached a state with more than 1 admissible advance! \n";

					return false;
				}
			}
		}
		for (auto it = Dq.begin(); it != Dq.end(); it++)
		{
		    if(posOfEl(*it,Adm) == -1)
            {
                Adm.push_back(*it);
            }
		}
		i++;
	}

	return true;
}

FSA<pair<string, string> > productFSA(const FSA<string>& a, const FSA<string>& b)
{
	set<Transition<string> > Edelt1 = a.delta;
	set<Transition<string> > Edelt2 = b.delta;
	for (int i = 1; i <= a.states; i++)
	{
		Edelt1.insert(Transition<string>(i, "", i));
	}
	for (int i = 1; i <= b.states; i++)
	{
		Edelt2.insert(Transition<string>(i, "", i));
	}
	unordered_map<int, vector<pair<string, int> > > delt1Prime = funcOneTwoThree(Edelt1);
	unordered_map<int, vector<pair<string, int> > > delt2Prime = funcOneTwoThree(Edelt2);

	vector<pair<int, int> > P;
	for (auto it = a.startingStates.begin(); it != a.startingStates.end(); it++)   ///I_1 x I_2
	{
		for (auto it2 = b.startingStates.begin(); it2 != b.startingStates.end(); it2++)
		{
			P.push_back({ *it,*it2 });
		}
	}
	set<Transition<pair<string, string> > > delta;
	int i = 0;
	while (i != P.size())
	{
		pair<int, int> ps = P[i];
		vector<pair<string, int> > D1 = delt1Prime[ps.first];
		vector<pair<string, int> > D2 = delt2Prime[ps.second];
		vector<pair<pair<string, string>, pair<int, int> > > N;
		for (auto it = D1.begin(); it != D1.end(); it++)///N = {((a,b),(q1,q2))|(a,q1) in D1 & (b,q2) in D2}
		{
			for (auto it2 = D2.begin(); it2 != D2.end(); it2++)
			{
				N.push_back({ {it->first,it2->first},{it->second,it2->second} });
			}
		}
		for (auto it = N.begin(); it != N.end(); it++)
		{
			if (posOfEl(it->second, P) == -1)
			{
				P.push_back(it->second);
			}
		}
		for (auto it = N.begin(); it != N.end(); it++)
		{
			delta.insert(Transition<pair<string, string> >(i + 1, it->first, posOfEl(it->second, P) + 1));
		}
		i++;
	}
	set<int> fins;
	set<int> starting;
	for (int i = 1; i <= P.size(); i++)
	{
		if (a.finalStates.find(P[i - 1].first) != a.finalStates.end() && b.finalStates.find(P[i - 1].second) != b.finalStates.end())
		{
			fins.insert(i);
		}
		if (a.startingStates.find(P[i - 1].first) != a.startingStates.end() && b.startingStates.find(P[i - 1].second) != b.startingStates.end())
		{
			starting.insert(i);
		}
	}

	return trimFSA(removeEpsilon(FSA<pair<string, string> >(P.size(), starting, fins, delta)));
}

FSA<pair<string, string> > replaceOPT(const set<string>& S, const FSA<pair<string, string> >& T)///Id(Sigma*).(T.Id(Sigma*))*
{
	FSA<pair<string, string> > N = identityFSA(forAllStar(S));
	FSA<pair<string, string> > result = concatFSA(N, starFSA(concatFSA(T, N)));

	return result;
}

FSA<pair<string, string> > replaceFST(const set<string>& S, const FSA<pair<string, string> >& T)///Id(Sigma*\(Sigma*.domT.Sigma*)) u <e,e> := N(T)....R(T) = N(T).(T.N(T))*
{
	FSA<string> all = forAllStar(S);
	FSA<string> allTall = concatFSA(concatFSA(all, domainFST(T)), all);
	FSA<pair<string, string> > NT = identityFSA(diffFSA(all, allTall));
	FSA<pair<string, string> > result = concatFSA(NT, starFSA(concatFSA(T, NT)));

	return result;
}

bool testConflictFreeness(const FSA<pair<string, string> >& T)
{
	set<string> Sigma;
	for (auto it = T.delta.begin(); it != T.delta.end(); it++)
	{
		if(it->word.first != "")
        {
            Sigma.insert(it->word.first);
        }
        if(it->word.second != "")
        {
            Sigma.insert(it->word.second);
        }
	}

	FSA<string> domT = domainFST(T);
	FSA<string> plusSigma = forAllPlus(Sigma);
	FSA<pair<string, string> > delSigma = productFSA(plusSigma, FSA<string>(""));
	FSA<string> prefixes = rangeFST(composeFST(identityFSA(domT), concatFSA(identityFSA(plusSigma), delSigma)));
	FSA<string> suffixes = rangeFST(composeFST(identityFSA(domT), concatFSA(delSigma, identityFSA(plusSigma))));
	FSA<string> all = forAllStar(Sigma);
	FSA<string> overLaps = trimFSA(intersectFSA(prefixes, suffixes));
	if(overLaps.states > 0)
    {
        cout << "There are overlapping words! \n";

        return false;
    }
	FSA<string> leftCont = concatFSA(concatFSA(plusSigma, domT), all);
	FSA<string> rightCont = concatFSA(all, concatFSA(domT, plusSigma));
	FSA<string> containments = trimFSA(intersectFSA(domT, unionFSA(leftCont, rightCont)));
	if(containments.states > 0)
    {
        cout << "There are containing words! \n";
        return false;
    }

    cout << "Conflicts tests successfully : \n";

	return true;
}

FSA<pair<string, string> > constructFST(const string& input, bool& isOptional)
{
	stack<FSA<pair<string, string> > > transducers;
	stack<char> op;
	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] == '`')
		{
			isOptional = true;
			continue;
		}
		else if (input[i] == '_')
		{
			FSA<pair<string, string> > a = FSA<pair<string, string> >();
			transducers.push(a);
			continue;
		}
		else if (input[i] == '<')
		{
			int j = i + 1;
			string word1;
			while (j + 1 < input.size() && input[j] != '^')
			{
				word1.push_back(input[j]);
				j++;
			}
			int k = j + 1;
			string word2;
			while (k + 1 < input.size() && input[k] != '>')
			{
				word2.push_back(input[k]);
				k++;
			}
			i = k;
			FSA<pair<string, string> > tr(make_pair(word1, word2));
			transducers.push(tr);
			continue;
		}
		else if (input[i] == '|' || input[i] == ':' || input[i] == '*' || input[i] == '+')
		{
			op.push(input[i]);
			continue;
		}
		else if (input[i] == '}')
		{
		    if(op.empty())
            {
                cout << "Tried to pop from an empty stack op : \n";
            }
			char action = op.top();
			op.pop();
			if (action == '|')
			{
			    if(transducers.empty())
                {
                    cout << "Tried to pop from an empty stack tr : \n";
                }
				FSA<pair<string, string> > first = transducers.top();
				transducers.pop();
				if(transducers.empty())
                {
                    cout << "Tried to pop from an empty stack tr : \n";
                }
				FSA<pair<string, string> > second = transducers.top();
				transducers.pop();
				FSA<pair<string, string> > res = unionFSA(first, second);
				transducers.push(res);
			}
			else if (action == ':')
			{
			    if(transducers.empty())
                {
                    cout << "Tried to pop from an empty stack tr : \n";
                }
				FSA<pair<string, string> > first = transducers.top();
				transducers.pop();
				if(transducers.empty())
                {
                    cout << "Tried to pop from an empty stack tr : \n";
                }
				FSA<pair<string, string> > second = transducers.top();
				transducers.pop();
				FSA<pair<string, string> > res = concatFSA(second, first);
				transducers.push(res);
			}
			else if (action == '*')
			{
			    if(transducers.empty())
                {
                    cout << "Tried to pop from an empty stack tr : \n";
                }
				FSA<pair<string, string> > first = transducers.top();
				transducers.pop();
				FSA<pair<string, string> > res = starFSA(first);
				transducers.push(res);
			}
			else if (action == '+')
            {
                if(transducers.empty())
                {
                    cout << "Tried to pop from an empty stack tr : \n";
                }
                FSA<pair<string, string> > first = transducers.top();
                transducers.pop();
                FSA<pair<string,string> > res = plusFSA(first);
                transducers.push(res);
            }
			else
			{
				cout << "Error in the expression \n";
			}
		}
	}
	FSA<pair<string, string> > result = transducers.top();
	transducers.pop();


	cout << "Successfully constructed : \n";

	return result;
}

bool testReplaceFunctionality(const FSA<pair<string, string> >& T)
{
    cout << "Testing replace functionality : \n";
	FSA<string> a = domainFST(T);
	FSA<string> includesEpsilon = trimFSA(intersectFSA(FSA<string>(""), a));
	if(includesEpsilon.states > 0)
    {
        cout << "includesEpsilon states are more than zero! \n";

        return false;
    }
	bool isFunc = testFunctionality(T);
	if(!isFunc)
    {
        cout << "Transducer is not functional : \n";
        return false;
    }

    cout << "Testing for conflicts : \n";

	return testConflictFreeness(T);
}

bool testOptionalFunctionality(const FSA<pair<string, string> >& a)
{
	for (auto it = a.delta.begin(); it != a.delta.end(); it++)
	{
		if (it->word.first != it->word.second)
		{
			return false;
		}
	}
	return true;
}

set<string> getDomAlphabet(const FSA<pair<string, string> >& a)
{
	set<string> al;
	for (auto it = a.delta.begin(); it != a.delta.end(); it++)
	{
		if(it->word.first != "")
		{
		al.insert(it->word.first);
		}
		if(it->word.second != "")
		{
			al.insert(it->word.second);
		}
	}

	return al;
}

void printQueue(queue<pair<int,string> > q)
{
    while(!q.empty())
    {
        auto v = q.front();
        q.pop();
        cout << v.first << " :" << v.second << ": " << endl;
    }
}

FSA<pair<string, string> > getFinalTransducer(const string& input, bool& isFunctional, const string& alpha)
{
	set<string> textChars;

	for (int i = 0; i < alpha.size(); i++)
	{
		textChars.insert(string(1, alpha[i]));
	}

	bool isOptional = false;
	FSA<pair<string, string> > a = constructFST(input, isOptional);
	FSA<pair<string, string> > exp = trimFSA(removeEpsilon(a));
	if (isOptional)
	{
		if (testOptionalFunctionality(exp))
		{
			FSA<pair<string, string> > b = replaceOPT(setUnion(textChars, getDomAlphabet(b)), exp);
			isFunctional = true;
			return b;
		}
		isFunctional = false;
		cout << "R^opt is not functional \n";
		return FSA<pair<string, string> >();
	}
	///cout << "Checking for replace functionality : \n";
	if (testReplaceFunctionality(exp))
	{
		isFunctional = true;
		///cout << "Time to replace : \n";
		FSA<pair<string, string> > b = replaceFST(setUnion(textChars, getDomAlphabet(exp)), exp);
		cout << "Successfully received replaceFST : \n";
		///cout << "Successfully replaced : \n";
		return b;
	}
	isFunctional = false;
	cout << "R(T) is not functional \n";
	return FSA<pair<string, string> >();
}

string getOutputString(const set<int>& finalStates, const set<Transition<pair<string, string> > >& delta, const string& input)
{
    cout << "Going for the output : \n";
	unordered_map<pair<int, string>, set<pair<string, int> > > adj;
	for (auto it = delta.begin(); it != delta.end(); it++)
	{
		if (adj.find({ it->start,it->word.first }) == adj.end())
		{
			set<pair<string, int> > s;
			s.insert({ it->word.second,it->finish });
			adj.insert({ {it->start,it->word.first},s });
		}
		else
		{
			adj[{it->start, it->word.first}].insert({ it->word.second,it->finish });
		}
	}
	///cout << "Printing delta : \n";
	queue<pair<int, string > > q;
	q.push({ 1,"" });
	string str;
	for (int i = 0; i < input.size(); i++)
	{
		queue<pair<int, string> > q2;
		while (!q.empty())
		{
			pair<int, string>  v = q.front();
			q.pop();
			for (auto it = adj[{v.first, string(1, input[i])}].begin(); it != adj[{v.first, string(1, input[i])}].end(); it++)
			{
				q2.push({ it->second,v.second + it->first });
			}
		}
		q = q2;
	}
	if (q.empty())
	{
		return str;
	}
	while(!q.empty())
    {
        auto v = q.front();
        q.pop();
        if(finalStates.find(v.first) != finalStates.end())
        {
            return v.second;
        }
    }


	return str;
}

void test1()
{
    string ones = "{{{{{{{{<I^1>|<II^2>}|<III^3>}|<IV^4>}|<V^5>}|<VI^6>}|<VII^7>}|<VIII^8>}|<IX^9>}";
	string ones0 = "{" + ones + "|<^0>}";
	string tens = "{{{{{{{{<X^1>|<XX^2>}|<XXX^3>}|<XL^4>}|<L^5>}|<LX^6>}|<LXX^7>}|<LXXX^8>}|<XC^9>}";
	string tens0 = "{" + tens + "|<^0>}";
	string hundreds = "{{{{{{{{<C^1>|<CC^2>}|<CCC^3>}|<CD^4>}|<D^5>}|<DC^6>}|<DCC^7>}|<DCCC^8>}|<CM^9>}";
	string hundreds0 = "{" + hundreds + "|<^0>}";
	string thousands = "{{<M^1>|<MM^2>}|<MMM^3>}";
	string tens0ones0 = "{" + tens0 + ":" + ones0 + "}";
	string hundreds0tens0ones0 = "{" + hundreds0 + ":" + tens0ones0 + "}";
	string thousandshundreds0tens0ones0 = "{" + thousands + ":" + hundreds0tens0ones0 + "}";
	string hundredtens0ones0 = "{" + hundreds + ":" + tens0ones0 + "}";
	string tensones0 = "{" + tens + ":" + ones0 + "}";
	string thousandsAndHundreds = "{" + thousandshundreds0tens0ones0 + "|" + hundredtens0ones0 + "}";
	string thousandsAndHundredsAndTens = "{" + thousandsAndHundreds + "|" + tensones0 + "}";
	string N = "{" + thousandsAndHundredsAndTens + "|" + ones + "}";
    string text = "The author's original plan, according to Gall., XVI. 1; XIX. 6; XXI. 1, was to include 20, but as Peter has pointed out (Abh. Sachs. Ges., XXVII. p. 190 f. MCDXLVIII.), this number was raised to that of the Thirty Tyrants of Athens by padding with ten additional names.";
    bool isFunctional = false;
	FSA<pair<string, string> > a = getFinalTransducer(N, isFunctional, text);
	if (isFunctional)
	{
		FSA<pair<string, string> > b = determinate(a);
		set<string> W;
		bool trash = false;
		FSA<pair<string,string> > c = realTime(b,W,trash);
		ofstream oFile;
		oFile.open("test1.txt");
		oFile << getOutputString(c.finalStates, c.delta, text) << endl;
		cout << "Successfully generated output test 1: \n";
	}
	else
	{
		cout << "Not functionaaal test 1: \n";
	}
}

void test2()
{
    string ones = "{{{{{{{{<I^1>|<II^2>}|<III^3>}|<IV^4>}|<V^5>}|<VI^6>}|<VII^7>}|<VIII^8>}|<IX^9>}";
	string ones0 = "{" + ones + "|<^0>}";
	string tens = "{{{{{{{{<X^1>|<XX^2>}|<XXX^3>}|<XL^4>}|<L^5>}|<LX^6>}|<LXX^7>}|<LXXX^8>}|<XC^9>}";
	string tens0 = "{" + tens + "|<^0>}";
	string hundreds = "{{{{{{{{<C^1>|<CC^2>}|<CCC^3>}|<CD^4>}|<D^5>}|<DC^6>}|<DCC^7>}|<DCCC^8>}|<CM^9>}";
	string hundreds0 = "{" + hundreds + "|<^0>}";
	string thousands = "{{<M^1>|<MM^2>}|<MMM^3>}";
	string tens0ones0 = "{" + tens0 + ":" + ones0 + "}";
	string hundreds0tens0ones0 = "{" + hundreds0 + ":" + tens0ones0 + "}";
	string thousandshundreds0tens0ones0 = "{" + thousands + ":" + hundreds0tens0ones0 + "}";
	string hundredtens0ones0 = "{" + hundreds + ":" + tens0ones0 + "}";
	string tensones0 = "{" + tens + ":" + ones0 + "}";
	string thousandsAndHundreds = "{" + thousandshundreds0tens0ones0 + "|" + hundredtens0ones0 + "}";
	string thousandsAndHundredsAndTens = "{" + thousandsAndHundreds + "|" + tensones0 + "}";
	string N = "{" + thousandsAndHundredsAndTens + "|" + ones + "}";
	string part2 = "{< ^ >:" + N + "}";
	string part3 = "{" + part2 + ": <.^.>}";
    string text = "Description Roman numerals are essentially a decimal or 'base ten' number system, but instead of place value notation (in which place-keeping zeros enable a digit to represent different powers of ten) the system uses a set of symbols with fixed values, including 'built in' powers of ten. Tally-like combinations of these fixed symbols correspond to the (placed) digits of Arabic numerals. This structure allows for significant flexibility in notation, and many variant forms are attested.  In fact, there has never been an officially binding, or universally accepted standard for Roman numerals. Usage in ancient Rome varied greatly and became thoroughly chaotic in medieval times. Even the post-renaissance restoration of a largely 'classical' notation has failed to produce total consistency: variant forms are even defended by some modern writers as offering improved 'flexibility'.[3] On the other hand, especially where a Roman numeral is considered a legally binding expression of a number, as in U.S. Copyright law (where an 'incorrect' or ambiguous numeral may invalidate a copyright claim, or affect the termination date of the copyright period)[4] it is desirable to strictly follow the usual style described below.  Standard form The following table displays how Roman Numerals are usually written:[5]  Individual decimal places Thousands Hundreds Tens Units 1 M C X I 2 MM CC XX II 3 MMM CCC XXX III 4  CD XL IV 5  D L V 6  DC LX VI 7  DCC LXX VII 8  DCCC LXXX VIII 9  CM XC IX The numerals for 4 (IV) and 9 (IX) are written using 'subtractive notation',[6] where the first symbol (I) is subtracted from the larger one (V, or X), thus avoiding the clumsier (IIII, and VIIII).[a] Subtractive notation is also used for 40 (XL) and 90 (XC), as well as 400 (CD) and 900 (CM).[7] These are the only subtractive forms in standard use.  A number containing several decimal digits is built by appending the Roman numeral equivalent for each, from highest to lowest, as in the following examples:     39 = XXX + IX = XXXIX.   246 = CC + XL + VI = CCXLVI.   789 = DCC + LXXX + IX = DCCLXXXIX. 2,421 = MM + CD + XX + I = MMCDXXI. Any missing place (represented by a zero in the place-value equivalent) is omitted, as in Latin (and English) speech:    160 = C + LX = CLX   207 = CC + VII = CCVII 1,009 = M + IX = MIX 1,066 = M + LX + VI = MLXVI[8][9] Roman numerals for large numbers are seen in the form of year numbers, as in these examples:  1776 = M + DCC + LXX + VI = MDCCLXXVI (the date written on the book held by the Statue of Liberty). 1918 = M + CM + X + VIII = MCMXVIII (the first year of the Spanish flu pandemic) 1954 = M + CM + L + IV = MCMLIV (as in the trailer for the movie The Last Time I Saw Paris)[4] 2014 = MM + X + IV = MMXIV (the year of the games of the XXII (22nd) Olympic Winter Games (in Sochi, Russia)) The largest number that can be represented in this notation is 3,999 (MMMCMXCIX), but since the largest Roman numeral likely to be required today is MMXXI (the current year) there is no practical need for larger Roman numerals. Prior to the introduction of Arabic numerals in the West, ancient and medieval users of the system used various means to write larger numbers; see Large numbers below.  Variant forms Forms exist that vary in one way or another from the general standard represented above.  Use of additive notation  A typical clock face with Roman numerals in Bad Salzdetfurth, Germany While subtractive notation for 4, 40 and 400 (IV, XL and CD) has been the usual form since Roman times, additive notation (IIII, XXXX and CCCC)[10] continued to be used, including in compound numbers like XXIIII,[11] LXXIIII,[12] and CCCCLXXXX.[13] The additive forms for 9, 90, and 900 (VIIII,[10] LXXXX,[14] and DCCCC[15]) have also been used, although less frequently.  The two conventions could be mixed in the same document or inscription, even in the same numeral. On the numbered gates to the Colosseum, for instance, IIII is systematically used instead of IV, but subtractive notation is used for other digits; so that gate 44 is labelled XLIIII.[16]  Modern clock faces that use Roman numerals still very often employ IIII for four o'clock but IX for nine o'clock, a practice that goes back to very early clocks such as the Wells Cathedral clock of the late 14th century.[17][18][19] However, this is far from universal: for example, the clock on the Palace of Westminster tower, Big Ben, uses a subtractive IV for 4 o'clock.[18]  Isaac Asimov once mentioned an 'interesting theory' that Romans avoided using IV because it was the initial letters of IVPITER, the Latin spelling of Jupiter, and might have seemed impious.[20] He did not say whose theory it was.   The year number on Admiralty Arch, London. The year 1910 is rendered as MDCCCCX, rather than the more usual MCMX Several monumental inscriptions created in the early 20th century use variant forms for '1900' (usually written MCM). These vary from MDCCCCX for 1910 as seen on Admiralty Arch, London, to the more unusual, if not unique MDCDIII for 1903, on the north entrance to the Saint Louis Art Museum.[21]  Especially on tombstones and other funerary inscriptions 5 and 50 have been occasionally written IIIII and XXXXX instead of V and L, and there are instances such as IIIIII and XXXXXX rather than VI or LX.[22][23]  Irregular subtractive notation There is a common belief that any smaller digit placed to the left of a larger digit is subtracted from the total, and that by clever choices a long Roman numeral can be 'compressed'. The best known example of this is the ROMAN() function in Microsoft Excel, which can turn 499 into CDXCIX, LDVLIV, XDIX, VDIV, or ID depending on the 'Form' setting.[24] There is no indication this is anything other than an invention by the programmer, and the universal-subtraction belief may be a result of modern users trying to rationalize the syntax of Roman numerals.   Epitaph of centurion Marcus Caelius, showing 'XIIX' There is however some historic use of subtractive notation other than that described in the above 'standard': in particular IIIXX for 17,[25] IIXX for 18,[26] IIIC for 97,[27] IIC for 98,[28][29] and IC for 99.[30] A possible explanation is that the word for 18 in Latin is duodeviginti, literally 'two from twenty', 98 is duodecentum (two from hundred), and 99 is undecentum (one from hundred).[31] However, the explanation does not seem to apply to IIIXX and IIIC, since the Latin words for 17 and 97 were septendecim (seven ten) and nonaginta septem (ninety seven), respectively.  There are multiple examples of IIX being used for 8. There does not seem to be a linguistic explanation for this use, although it is one stroke shorter than VIII. XIIX was used by officers of the XVIII Roman Legion to write their number.[32][33] The notation appears prominently on the cenotaph of their senior centurion Marcus Caelius (c. 45 BC – AD 9). On the publicly displayed official Roman calendars known as Fasti, XIIX is used for the 18 days to the next Kalends, and XXIIX for the 28 days in February. The latter can be seen on the sole extant pre-Julian calendar, the Fasti Antiates Maiores.[34]  Rare variants While irregular subtractive and additive notation has been used at least occasionally throughout history, some Roman numerals have been observed in documents and inscriptions that do not fit either system. Some of these variants do not seem to have been used outside specific contexts, and may have been regarded as errors even by contemporaries.   Padlock used on the north gate of the Irish town of Athlone. '1613' in the date is rendered XVIXIII, (literally '16, 13') instead of MDCXIII IIXX was how people associated with the XXII Roman Legion used to write their number. The practice may have been due to a common way to say 'twenty-second' in Latin, namely duo et vice(n)sima (literally 'two and twentieth') rather than the 'regular' vice(n)sima secunda (twenty second).[35] Apparently, at least one ancient stonecutter mistakenly thought that the IIXX of '22nd Legion' stood for 18, and 'corrected' it to XVIII.[35]  Excerpt from Bibliothèque nationale de France.[36] The Roman numeral for 500 is rendered as VC, instead of D There are some examples of year numbers after 1000 written as two Roman numerals 1–99, e.g. 1613 as XVIXIII, corresponding to the common reading 'sixteen thirteen' of such year numbers in English, or 1519 as XVCXIX as in French quinze-cent-dix-neuf (fifteen-hundred and nineteen), and similar readings in other languages.[37] In some French texts from the 15th century and later one finds constructions like IIIIXXXIX for 99, reflecting the French reading of that number as quatre-vingt-dix-neuf (four-score and nineteen).[37] Similarly, in some English documents one finds, for example, 77 written as 'iiixxxvii' (which could be read 'three-score and seventeen').[38] Another medieval accounting text from 1301 renders numbers like 13,573 as 'xiii. m. v. c. iii. xx. xiii', that is, '13×1000 + 5×100 + 3×20 + 13'.[39] Other numerals that do not fit the usual patterns – such as VXL for 45, instead of the usual XLV — may be due to scribal errors, or the writer's lack of familiarity with the system, rather than being genuine variant usage. Non-numeric combinations As Roman numerals are composed of ordinary alphabetic characters, there may sometimes be confusion with other uses of the same letters. For example, 'XXX' and 'XL' have other connotations in addition to their values as Roman numerals, while 'IXL' more often than not is a gramogram of 'I excel', and is in any case not an unambiguous Roman numeral.";
    bool isFunctional = false;
	FSA<pair<string, string> > a = getFinalTransducer(part3, isFunctional, text);
	if (isFunctional)
	{
		FSA<pair<string, string> > b = determinate(a);
		set<string> W;
		bool trash = false;
		FSA<pair<string,string> > c = realTime(b,W,trash);
		ofstream oFile;
		oFile.open("test2.txt");
		oFile << getOutputString(c.finalStates, c.delta, text) << endl;
		cout << "Successfully generated output test 2: \n";
	}
	else
	{
		cout << "Not functionaaal test 2: \n";
	}
}

void test3()
{
    string ones = "{{{{{{{{<I^1>|<II^2>}|<III^3>}|<IV^4>}|<V^5>}|<VI^6>}|<VII^7>}|<VIII^8>}|<IX^9>}";
	string ones0 = "{" + ones + "|<^0>}";
	string tens = "{{{{{{{{<X^1>|<XX^2>}|<XXX^3>}|<XL^4>}|<L^5>}|<LX^6>}|<LXX^7>}|<LXXX^8>}|<XC^9>}";
	string tens0 = "{" + tens + "|<^0>}";
	string hundreds = "{{{{{{{{<C^1>|<CC^2>}|<CCC^3>}|<CD^4>}|<D^5>}|<DC^6>}|<DCC^7>}|<DCCC^8>}|<CM^9>}";
	string hundreds0 = "{" + hundreds + "|<^0>}";
	string thousands = "{{<M^1>|<MM^2>}|<MMM^3>}";
	string tens0ones0 = "{" + tens0 + ":" + ones0 + "}";
	string hundreds0tens0ones0 = "{" + hundreds0 + ":" + tens0ones0 + "}";
	string thousandshundreds0tens0ones0 = "{" + thousands + ":" + hundreds0tens0ones0 + "}";
	string hundredtens0ones0 = "{" + hundreds + ":" + tens0ones0 + "}";
	string tensones0 = "{" + tens + ":" + ones0 + "}";
	string thousandsAndHundreds = "{" + thousandshundreds0tens0ones0 + "|" + hundredtens0ones0 + "}";
	string thousandsAndHundredsAndTens = "{" + thousandsAndHundreds + "|" + tensones0 + "}";
	string p1 = "{" + thousands + ":" + hundreds0 + "}";
	string p2 = "{" + p1 + ":" + tens0 + "}";
	string p3 = "{" + p2 + ":" + ones + "}";
	string p4 = "{" + p3 + "+" + "}";
    string text = "The author's original plan, according to Gall., XVI. 1; XIX. 6; XXI. 1, was to include 20, but as Peter has pointed out (Abh. Sachs. Ges., XXVII. p. 190 f. MCDXLVIII.), this number was raised to that of the Thirty Tyrants of Athens by padding with ten additional names.";
    bool isFunctional = false;
	FSA<pair<string, string> > a = getFinalTransducer(p4, isFunctional, text);
	if (isFunctional)
	{
		FSA<pair<string, string> > b = determinate(a);
		set<string> W;
		bool trash = false;
		FSA<pair<string,string> > c = realTime(b,W,trash);
		ofstream oFile;
		oFile.open("test3.txt");
		oFile << getOutputString(c.finalStates, c.delta, text) << endl;
		cout << "Successfully generated output test 3: \n";
	}
	else
	{
		cout << "Not functionaaal test 3: \n";
	}

}

void test4()
{
    string ones = "{{{{{{{{<I^1>|<II^2>}|<III^3>}|<IV^4>}|<V^5>}|<VI^6>}|<VII^7>}|<VIII^8>}|<IX^9>}";
	string ones0 = "{" + ones + "|<^0>}";
	string tens = "{{{{{{{{<X^1>|<XX^2>}|<XXX^3>}|<XL^4>}|<L^5>}|<LX^6>}|<LXX^7>}|<LXXX^8>}|<XC^9>}";
	string tens0 = "{" + tens + "|<^0>}";
	string hundreds = "{{{{{{{{<C^1>|<CC^2>}|<CCC^3>}|<CD^4>}|<D^5>}|<DC^6>}|<DCC^7>}|<DCCC^8>}|<CM^9>}";
	string hundreds0 = "{" + hundreds + "|<^0>}";
	string thousands = "{{<M^1>|<MM^2>}|<MMM^3>}";
	string tens0ones0 = "{" + tens0 + ":" + ones0 + "}";
	string hundreds0tens0ones0 = "{" + hundreds0 + ":" + tens0ones0 + "}";
	string thousandshundreds0tens0ones0 = "{" + thousands + ":" + hundreds0tens0ones0 + "}";
	string hundredtens0ones0 = "{" + hundreds + ":" + tens0ones0 + "}";
	string tensones0 = "{" + tens + ":" + ones0 + "}";
	string thousandsAndHundreds = "{" + thousandshundreds0tens0ones0 + "|" + hundredtens0ones0 + "}";
	string thousandsAndHundredsAndTens = "{" + thousandsAndHundreds + "|" + tensones0 + "}";
	string thousands1 = "<MM^2>";
	string ones1 = "<V^5>";
	string p1 = "< ^ >";
	string p2 = "{" + thousands1 + "*" + "}";
	string p3 = "{" + p1 + ":" + p2 + "}";
	string p4 = "{" + p3 + ":" + hundreds0 + "}";
	string p5 = "{" + p4 + ":" + tens0 + "}";
	string p6 = "{" + p5 + ":" + ones1 + "}";
	string text = "The author's original plan, according to Gall., XVI. 1; XIX. 6; XXI. 1, was to include 20, but as Peter has pointed out (Abh. Sachs. Ges., XXVII. p. 190 f. MMMMCDXLVMMCDXLVI.), this number was raised to that of the Thirty Tyrants of Athens by padding with ten additional names.";

    bool isFunctional = false;
	FSA<pair<string, string> > a = getFinalTransducer(p6, isFunctional, text);
	if (isFunctional)
	{
		FSA<pair<string, string> > b = determinate(a);
		set<string> W;
		bool trash = false;
		FSA<pair<string,string> > c = realTime(b,W,trash);
		ofstream oFile;
		oFile.open("test4.txt");
		oFile << getOutputString(c.finalStates, c.delta, text) << endl;
		cout << "Successfully generated output test 4: \n";
	}
	else
	{
		cout << "Not functionaaal test 4: \n";
	}
}




int main()
{

    test2();

	return 0;
}

