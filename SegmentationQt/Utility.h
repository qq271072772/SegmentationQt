
#if !defined IS_DICTIONARY
#define IS_DICTIONARY

#include <map>
#include<vector>

namespace IS{
	template <typename Key, typename Value>
	class Dictionary : std::map<Key,Value>{

	private:

		class Item{
		private:
			iterator ite;
			std::map<Key, Value>& mp;
		public:
			Item(map<Key,Value>& m , iterator i) : mp(m),ite(i){};
			void operator=(Value i){
				mp[ite->first] = i;
			}
			operator Value(){
				return ite->second;
			}
		};

	public:
		Dictionary(){};
		~Dictionary(){};

		void Add(Key key, Value value){
			insert(std::pair<Key, Value>(key, value));
		}
		bool ContainsKey(Key key){
			return (find(key) != end());
		}
		int Count(){
			return size();
		}
		void Remove(Key key){
			erase(key);
		}
		void Clear(){
			clear();
		}
		Item Get(Key key){
			iterator ite = find(key);
			if (ite == end())
				throw "Key not found in Item() call";
			return (Item(*this, ite));
		}
		Item operator[](Key key){
			//Error : int x = dict[3],key = 3 not in dict but added
			//if (!ContainsKey(key))
			//	Add(key, NULL);
			return Get(key);
		}
		Value operator()(Key key){
			iterator ite = find(key);
			if (ite == end())
				throw "Key not found in Item() call";
			return ite->second;
		}

		std::vector<Key>Keys() 
		{
			std::vector<Key>keys;
			std::transform(begin(), end(), std::back_inserter(keys), getKEY);
			return keys;
		}

		static Key getKEY(std::pair<Key, Value> p)
		{
			return p.first;
		}
	};
}

#endif