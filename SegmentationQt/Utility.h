
#if !defined IS_DICTIONARY
#define IS_DICTIONARY

#include <map>
#include<vector>

namespace IS{

	template<typename Value>
	class List : std::vector<Value>{
	private:

		class Item{
		private:
			iterator ite;
		public:
			Item(iterator i) :ite(i){};
			void operator=(Value v){
				*ite = v;
			}
			operator Value(){
				return *ite;
			}
		};

		int Find(Value value){
			int index = 0;
			vector<Value>::iterator ite;
			for (ite = begin(); ite != end(); ite++){
				if (*ite == value){
					return index;
				}
				index++;
			}
			return -1;
		}

	public:
		void Add(Value value){
			vector<Value> v;

			push_back(value);
		}
		void Insert(Value value, int index){
			if (index < 0)
				index = 0;
			if (index = size())
				index = size();
			insert(begin() + index, value);
		}
		bool Contains(Value value){
			return Find(value) >= 0;
		}
		void Remove(Value value){
			int index = Find(value);
			if (index < 0)
				return;
			erase(begin() + index);
		}
		void RemoveAt(int index){
			if (index >= 0 && index < size())
				erase(begin() + index);
		}
		int Count(){
			return size();
		}
		void Clear(){
			clear();
		}

		Item operator[](int index){
			if (index < 0 || index >= size())
				throw "Index out of range exception";
			return (Item(begin() + index));
		}
		Value Get(int index){
			return static_cast<Value>(Item(begin() + index));
		}
		//Value operator()(int index){
		//	if (index < 0 || index >= size())
		//		throw "Index out of range exception";
		//	return (*(begin() + index));
		//}
	};

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

		//May be interferential
		//Value operator()(Key key){
		//	iterator ite = find(key);
		//	if (ite == end())
		//		throw "Key not found in Item() call";
		//	return ite->second;
		//}
		Value GetCasted(Key key){
			Item item = Get(key);
			return static_cast<Value>(item);
		}

		List<Key> Keys()
		{
			List<Key>keys;
			map<Key,Value>::iterator ite;
			for (ite = begin(); ite != end(); ite++){
				keys.Add(ite->first);
			}
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