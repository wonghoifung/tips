#include <iostream>
#include <string>
#include <assert.h>
#include "leveldb/db.h"
#include "leveldb/write_batch.h"

using namespace std;

static void checkKey(const string& key, leveldb::DB* db) {
  string v;
  leveldb::Status status = db->Get(leveldb::ReadOptions(), key, &v);
  assert(status.ok());
  cout << v << endl;
}

int main() {
  leveldb::DB* db;
  leveldb::Options options;
  options.create_if_missing = true;

  // open
  leveldb::Status status = leveldb::DB::Open(options, "./testdb", &db);
  assert(status.ok());

  // writebatch
  leveldb::WriteBatch batch;
  batch.Put("age", "32");
  batch.Delete("age");
  batch.Put("age", "31");
  batch.Put("sex", "male");
  status = db->Write(leveldb::WriteOptions(), &batch);
  assert(status.ok());
  checkKey("age", db);
  checkKey("sex", db);

  string key = "name";
#if 0
  // put
  string value = "albert";
  status = db->Put(leveldb::WriteOptions(), key, value);
  assert(status.ok());
#endif
  // get
  string v;
  status = db->Get(leveldb::ReadOptions(), key, &v);
  assert(status.ok());
  cout << v << endl;

#if 0
  // delete
  status = db->Delete(leveldb::WriteOptions(), key);
  assert(status.ok());
  status = db->Get(leveldb::ReadOptions(), key, &v);
  if (!status.ok()) {
    cout << key << " " << status.ToString() << endl;
  } else {
    cout << key << ":" << v << endl;
  }
#endif
  // close
  delete db;

  return 0;
}

