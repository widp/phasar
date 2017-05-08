/*
 * Table.hh
 *
 *  Created on: 07.11.2016
 *      Author: pdschbrt
 */

#ifndef UTILS_TABLE_HH_
#define UTILS_TABLE_HH_

#include <iostream>
#include <set>
#include <unordered_map>
#include <tuple>
using namespace std;


template<class R, class C, class V>
class Table {
private:
	unordered_map<R, unordered_map<C, V>> table;

public:
	struct Cell {
		R r;
		C c;
		V v;
		Cell(R row, C col, V val) : r(row), c(col), v(val) { }
		R getRowKey() { return r; }
		C getColumnKey() { return c; }
		V getValue() { return v; }
		friend ostream& operator<< (ostream& os, const Cell& c) { return os << "Cell: " << c.r << ", " << c.c << ", " << c.v; }
		friend bool operator< (const Cell& lhs, const Cell& rhs) { return tie(lhs.r, lhs.c, lhs.v) < tie(rhs.r, rhs.c, rhs.v); }
		friend bool operator== (const Cell& lhs, const Cell& rhs) { return tie(lhs.r, lhs.c, lhs.v) == tie(rhs.r, rhs.c, rhs.v); }
	};

	Table() = default;

	~Table() = default;

	Table(const Table& t) = default;

	Table(Table&& t) = default;

	Table& operator= (const Table& t) = default;

	Table& operator= (Table&& t) = default;

	V insert(R r, C c, V v)
	{
		// Associates the specified value with the specified keys.
		table[r][c] = v;
		return v;
	}

	void clear() { table.clear(); }

	bool isEmpty() { return table.empty(); }

	size_t size() { return table.size(); }

	set<Cell> cellSet()
	{
		// Returns a set of all row key / column key / value triplets.
		set<Cell> s;
		for (auto& m1 : table) {
			for (auto& m2 : m1.second) {
				s.insert(Cell(m1.first, m2.first, m2.second));
			}
		}
		return s;
	}

	unordered_map<R, V>	column(C columnKey)
	{
		// Returns a view of all mappings that have the given column key.
		unordered_map<R, V> column;
		for (auto& row : table) {
			if (row.second.count(columnKey))
				column[row.first] = row.second[columnKey];
		}
		return column;
	}

	multiset<C> columnKeySet()
	{
		// Returns a set of column keys that have one or more values in the table.
		multiset<C> colkeys;
		for (auto& m1 : table)
			for (auto& m2 : m1.second)
				colkeys.insert(m2.first);
		return colkeys;
	}

	unordered_map<C, unordered_map<R, V>> columnMap()
	{
		// Returns a view that associates each column key with the corresponding map from row keys to values.
		unordered_map<C, unordered_map<R, V>> columnmap;
		for (auto& m1 : table) {
			for (auto& m2 : table.second) {
				columnmap[m2.first][m1.first] = m2.second;
			}
		}
		return columnmap;
	}

	bool contains(R rowKey, C columnKey)
	{
		// Returns true if the table contains a mapping with the specified row and column keys.
		if(table.count(rowKey))
			return table[rowKey].count(columnKey);
		return false;
	}

	bool containsColumn(C columnKey)
	{
		// Returns true if the table contains a mapping with the specified column.
		for (auto& m1 : table)
			if (m1.second.count(columnKey))
				return true;
		return false;
	}

	bool containsRow(R rowKey)
	{
		// Returns true if the table contains a mapping with the specified row key.
		return table.count(rowKey);
	}

	bool containsValue(V value)
	{
		// Returns true if the table contains a mapping with the specified value.
		for (auto& m1 : table)
			for (auto& m2 : m1.second)
				if (value == m2.second)
					return true;
		return false;
	}

	V& get(R rowKey, C columnKey)
	{
		// Returns the value corresponding to the given row and column keys, or null if no such mapping exists.
			return table[rowKey][columnKey];
	}

	V remove(R rowKey, C columnKey)
	{
		// Removes the mapping, if any, associated with the given keys.
		V v = table[rowKey][columnKey];
		table[rowKey].erase(columnKey);
		return v;
	}

	unordered_map<C, V>& row(R rowKey)
	{
		// Returns a view of all mappings that have the given row key.
		return table[rowKey];
	}

	multiset<R> rowKeySet()
	{
		// Returns a set of row keys that have one or more values in the table.
		multiset<R> s;
		for (auto& m1 : table)
			s.insert(m1.first);
		return s;
	}

	unordered_map<R, unordered_map<C, V>> rowMap()
	{
		// Returns a view that associates each row key with the corresponding map from column keys to values.
		return table;
	}

	multiset<V> values()
	{
		// Returns a collection of all values, which may contain duplicates.
		multiset<V> s;
		for (auto& m1 : table)
			for (auto& m2 : m1.second)
				s.insert(m2.second);
		return s;
	}

	friend bool operator== (const Table<R,C,V>& lhs, const Table<R,C,V>& rhs) { return lhs.table == rhs.table; }

	friend bool operator< (const Table<R,C,V>& lhs, const Table<R,C,V>& rhs) { return lhs.table < rhs.table; }

	friend ostream& operator<< (ostream& os, const Table<R, C, V>& t)
	{
		for (auto& m1 : t.table)
			for (auto &m2 : m1.second)
				os << "< " << m1.first << " , " << m2.first << " , " << m2.second << " >\n";
		return os;
	}
};

#endif /* UTILS_TABLE_HH_ */