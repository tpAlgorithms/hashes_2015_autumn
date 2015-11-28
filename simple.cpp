#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
//#include <stdint.h>
#include <algorithm>

typedef std::string name_t;
typedef std::string value_t;
static const std::string UNDEFINED = "__undefined";
static const std::string DELETED = "__deleted";

struct item_t {
  name_t key;
  value_t value;
};

item_t make_item(const char*key, const char* value) {
  item_t item;
  item.key = key;
  item.value = value;
  return item;
}
 
size_t calc_hash(const name_t &name) {
  size_t factor = 12931127;
  size_t base = 69782751;
  size_t sum = 212638627 % base;
  for (size_t pos = 0; pos < name.size(); ++pos) {
    sum = (static_cast<uint8_t>(name[pos]) + sum * factor) % base;
  }
  return sum;
}
  
struct hash_table_t {
  std::vector<item_t> data;
  size_t used;
};
void init(hash_table_t &table, size_t sz);

size_t _add(hash_table_t &table, const item_t &item) {
  
  size_t hash = calc_hash(item.key);
  //printf("HASH: %s %zu\n", item.key.c_str(), hash);
  for (size_t probe = 0; probe < table.data.size(); ++probe) { 
    size_t pos = (hash + probe) % table.data.size();
    //printf("CHECK_PROBE: %zu %s %zu\n", probe, item.key.c_str(), pos);
    if (table.data[pos].key == UNDEFINED ||
        table.data[pos].key == DELETED) { 
      table.data[pos] = item;
      table.used += 1;
      return probe + 1;
    }
  }
  return table.data.size();
}


size_t add(hash_table_t &table, const item_t &item) {
  size_t count = 0;
  if (table.used * 4 >= table.data.size() * 3) {
    hash_table_t tmp;
    init(tmp, table.data.size() * 2);
    for (size_t pos = 0; pos < table.data.size(); ++pos) {
      if (table.data[pos].key != UNDEFINED &&
          table.data[pos].key != DELETED) {
        count += _add(tmp, table.data[pos]);
      }
    }

    std::swap(table, tmp);
  }
   
  count += _add(table, item);
  return count;
}

void dump(const hash_table_t &table) {
  for (size_t pos = 0; pos < table.data.size(); ++pos) {
    std::cout << "{" << table.data[pos].key << "=>" << table.data[pos].value << "}" << std::endl; 
  }
}

void init(hash_table_t &table, size_t sz) {
  table.data = std::vector<item_t>(sz, make_item(UNDEFINED.c_str(), ""));
  table.used = 0;
}
  
int main() {
  const size_t BUFF_SZ = 1024;
  char buffer[BUFF_SZ];


  hash_table_t table;
  init(table, 10);

  while(!feof(stdin)) {
    char *out = fgets(buffer, BUFF_SZ, stdin);
    if (!out) break;

    char key[BUFF_SZ];
    char value[BUFF_SZ];
    if (sscanf(out, "%s%s", key, value) ==2) {
      size_t count = add(table, make_item(key, value));
      printf("%zu\n", count);
      //printf("[%s => %s]\n", key, value);
    }
  }


  //printf("TABLE[%zu]:\n", table.data.size());
  //dump(table);
  return 0;
}





























 
