
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
			Key key;
		public:
			Item(map<Key,Value>& m , iterator i,Key k) : mp(m),ite(i),key(k){};
			void operator=(Value i){
				mp[ite == mp.end() ? key : ite->first] = i;
			}
			operator Value(){
				if (ite == mp.end())
					throw "Key not found exception.";
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
			return (Item(*this, ite,key));
		}
		Item operator[](Key key){
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

	protected:
		static Key getKEY(std::pair<Key, Value> p)
		{
			return p.first;
		}
	};
}

#endif