#include "tastylib/TextQuery.h"
#include <sstream>
#include <algorithm>
#include <iterator>

TASTYLIB_NS_BEGIN

// TextQuery

TextQuery::TextQuery(std::istream& is) : txt(new std::vector<std::string>) {
    std::string line;
    while (std::getline(is, line)) {
        txt->push_back(line);
        // Replace punctuations with spaces
        std::replace_if(line.begin(), line.end(), isPunc, ' ');
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            auto &lines = wm[word];
            if (!lines) {
                lines.reset(new std::set<LineNo>);
            }
            lines->insert(txt->size() - 1);
        }
    }
}

TextQuery::QueryResult TextQuery::query(const std::string& word) const {
    static std::shared_ptr<std::set<LineNo>> empty(new std::set<LineNo>);
    auto it = wm.find(word);
    if (it == wm.end()) {
        return QueryResult(word, txt, empty);
    } else {
        return QueryResult(word, txt, it->second);
    }
}

bool TextQuery::isPunc(const char c) noexcept {
    return c == '.' || c == ',' || c == '?' || c == '\'' || c == '\"';
}

// QueryResult

TextQuery::QueryResult::QueryResult(
    const std::string& s,
    std::shared_ptr<std::vector<std::string>> t,
    std::shared_ptr<std::set<LineNo>> l)
: queryStr(s), txt(t), lines(l) {}

std::ostream& operator<<(std::ostream& os, const TextQuery::QueryResult& res) {
    os << res.queryStr << " occurs: " << res.lines->size() << "\n";
    for (const auto line : *res.lines) {
        os << "(line " << line + 1 << ") " << res.txt->at(line) << "\n";
    }
    return os;
}

// Query

Query::Query(const std::string& word) : p(new WordQuery(word)) {}

Query::Query(const std::shared_ptr<QueryBase> p_) noexcept : p(p_) {}

TextQuery::QueryResult Query::eval(const TextQuery& tq) const {
    return p->eval(tq);
}

std::string Query::rep() const {
    return p->rep();
}

Query operator~(const Query& q) {
    return Query(std::shared_ptr<NotQuery>(new NotQuery(q)));
}

Query operator&(const Query& lhs, const Query& rhs) {
    return Query(std::shared_ptr<AndQuery>(new AndQuery(lhs, rhs)));
}

Query operator|(const Query& lhs, const Query& rhs) {
    return Query(std::shared_ptr<OrQuery>(new OrQuery(lhs, rhs)));
}

// WordQuery

WordQuery::WordQuery(const std::string& w) : word(w) {}

TextQuery::QueryResult WordQuery::eval(const TextQuery& tq) const {
    return tq.query(word);
}

std::string WordQuery::rep() const {
    return word;
}

// NotQuery

NotQuery::NotQuery(const Query& q) : query(q) {}

TextQuery::QueryResult NotQuery::eval(const TextQuery& tq) const {
    TextQuery::QueryResult ori = query.eval(tq);
    auto resLines = std::make_shared<std::set<LineNo>>();
    auto lines = ori.lines;
    auto numLines = ori.txt->size();
    auto it = lines->cbegin(), end = lines->cend();
    for (decltype(numLines) i = 0; i != numLines; ++i) {
        if (it == end || i != *it) {
            resLines->insert(i);
        } else {
            ++it;
        }
    }
    return TextQuery::QueryResult(rep(), ori.txt, resLines);
}

std::string NotQuery::rep() const {
    return "~(" + query.rep() + ")";
}

// BinaryQuery

BinaryQuery::BinaryQuery(const Query& l, const Query& r, const std::string& o)
: lhs(l), rhs(r), op(o) {}

std::string BinaryQuery::rep() const {
    return "(" + lhs.rep() + " " + op + " " + rhs.rep() + ")";
}

// OrQuery

OrQuery::OrQuery(const Query& l, const Query& r) : BinaryQuery(l, r, "|") {}

TextQuery::QueryResult OrQuery::eval(const TextQuery& tq) const {
    TextQuery::QueryResult lRes = lhs.eval(tq);
    TextQuery::QueryResult rRes = rhs.eval(tq);
    auto resLines = std::make_shared<std::set<LineNo>>(*lRes.lines);
    resLines->insert(rRes.lines->cbegin(), rRes.lines->cend());
    return TextQuery::QueryResult(rep(), lRes.txt, resLines);
}

// AndQuery

AndQuery::AndQuery(const Query& l, const Query& r) : BinaryQuery(l, r, "&") {}

TextQuery::QueryResult AndQuery::eval(const TextQuery& tq) const {
    TextQuery::QueryResult lRes = lhs.eval(tq);
    TextQuery::QueryResult rRes = rhs.eval(tq);
    auto resLines = std::make_shared<std::set<LineNo>>();
    std::set_intersection(lRes.lines->cbegin(), lRes.lines->cend(),
                          rRes.lines->cbegin(), rRes.lines->cend(),
                          std::inserter(*resLines, resLines->cbegin()));
    return TextQuery::QueryResult(rep(), lRes.txt, resLines);
}

TASTYLIB_NS_END
