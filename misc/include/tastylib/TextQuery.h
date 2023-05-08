/*
Implementations of the Text-Query program from §12.3 & §15.9, C++ Primer, 5th Edition.
*/
#ifndef TASTYLIB_TEXTQUERY_H_
#define TASTYLIB_TEXTQUERY_H_

#include "tastylib/internal/base.h"
#include <vector>
#include <string>
#include <set>
#include <map>
#include <memory>
#include <iostream>

TASTYLIB_NS_BEGIN

// Search words from a given input stream
class TextQuery {
public:
    using LineNo = std::vector<std::string>::size_type;

    class QueryResult;

public:
    explicit TextQuery(std::istream& is);

    QueryResult query(const std::string& word) const;

private:
    static bool isPunc(const char c) noexcept;

private:
    // Store each line in the text
    std::shared_ptr<std::vector<std::string>> txt;
    // Map words to line numbers
    std::map<std::string, std::shared_ptr<std::set<LineNo>>> wm;
};

class NotQuery;
class OrQuery;
class AndQuery;

// Query result from TextQuery
class TextQuery::QueryResult {
    friend class NotQuery;
    friend class OrQuery;
    friend class AndQuery;

    friend std::ostream& operator<<(std::ostream& os, const QueryResult& res);

public:
    QueryResult(const std::string& s,
                std::shared_ptr<std::vector<std::string>> t,
                std::shared_ptr<std::set<LineNo>> l);

private:
    std::string queryStr;  // Word this query represents
    std::shared_ptr<std::vector<std::string>> txt;
    std::shared_ptr<std::set<LineNo>> lines;
};

class QueryBase;

// Public interface for query
class Query {
    friend Query operator~(const Query& q);
    friend Query operator&(const Query& lhs, const Query& rhs);
    friend Query operator|(const Query& lhs, const Query& rhs);

public:
    explicit Query(const std::string& word);  // Create a WordQuery

    TextQuery::QueryResult eval(const TextQuery& tq) const;

    std::string rep() const;

private:
    explicit Query(const std::shared_ptr<QueryBase> p_) noexcept;

private:
    std::shared_ptr<QueryBase> p;
};

// Base class for different query types
class QueryBase {
    friend class Query;

protected:
    using LineNo = TextQuery::LineNo;

    virtual ~QueryBase() = default;

private:
    virtual TextQuery::QueryResult eval(const TextQuery& tq) const = 0;
    virtual std::string rep() const = 0;
};

// Query for words
class WordQuery: public QueryBase {
    friend class Query;

private:
    explicit WordQuery(const std::string& w);

    virtual TextQuery::QueryResult eval(const TextQuery& tq) const;

    virtual std::string rep() const;

private:
    std::string word;
};

// NOT logic query
class NotQuery: public QueryBase {
    friend Query operator~(const Query& q);

private:
    explicit NotQuery(const Query& q);

    virtual TextQuery::QueryResult eval(const TextQuery& tq) const;

    virtual std::string rep() const;

private:
    Query query;
};

// Base class for two-operand query
class BinaryQuery: public QueryBase {
protected:
    BinaryQuery(const Query& l, const Query& r, const std::string& o);

    virtual std::string rep() const;

protected:
    Query lhs;
    Query rhs;
    std::string op;
};

// OR logic query
class OrQuery: public BinaryQuery {
    friend Query operator|(const Query& lhs, const Query& rhs);

private:
    OrQuery(const Query& l, const Query& r);

    virtual TextQuery::QueryResult eval(const TextQuery& tq) const;
};

// AND logic query
class AndQuery: public BinaryQuery {
    friend Query operator&(const Query& lhs, const Query& rhs);

private:
    AndQuery(const Query& l, const Query& r);

    virtual TextQuery::QueryResult eval(const TextQuery& tq) const;
};

TASTYLIB_NS_END

#endif
