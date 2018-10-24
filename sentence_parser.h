#pragma once

#include <string>
#include <vector>

class sentence_parser {
public:
    sentence_parser(const sentence_parser&)=default;
    sentence_parser(sentence_parser&&)=default;
    sentence_parser& operator=(const sentence_parser&)=default;
    sentence_parser& operator=(sentence_parser&&)=default;

    explicit sentence_parser(const std::string &file_location);

    std::vector<std::string> operator()() const;

private:
    void load_document_(const std::string &file_location);
    void find_paragraph_breaks_(std::vector <std::string::size_type> &breaks) const;
    void find_sentence_breaks_(std::vector <std::string::size_type> &breaks) const;
    void find_period_breaks_(std::vector <std::string::size_type> &breaks) const;

    bool is_skip_word_(std::string::size_type pos) const;

    std::string data;
};