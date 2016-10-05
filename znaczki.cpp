/*
=== Zadanie znaczki pocztowe ===
            JNP I


Marek Paweł Drozdowski  370778
Hubert Banaszewski      370712
*/


#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <tuple>
#include <iterator>
#include <sstream>
#include <algorithm>

#define Stamp tuple<string, string, string, string>
#define Interval pair<string, string>

#define space ' '
#define beginning_of_the_error_message "Error in line " 
#define colon ':'
#define empty_string ""
#define zero '0'
#define dot '.'
#define comma ','

#define year_stamp(year) make_tuple(year, empty_string, empty_string, empty_string)

using namespace std;

set<Stamp> stamps; 


void print_stamp(const Stamp& stamp) {
    cout << get<0>(stamp) << space;
    cout << get<1>(stamp) << space;
    cout << get<2>(stamp) << space;
    cout << get<3>(stamp) << endl;
};

void print_error_message(int line_count, string line) {
    cout << beginning_of_the_error_message << line_count << colon << line << endl;
}


void print_stamp_interval(string begin_year, string end_year) {
    set<Stamp>::iterator low, up;
	Stamp min_stamp = make_tuple(begin_year, "", "", "");
	Stamp max_stamp = make_tuple(end_year, "", "", "");  
    low = lower_bound(stamps.begin(), stamps.end(), min_stamp);
    up = upper_bound(stamps.begin(), stamps.end(), max_stamp);

    for (; low != up; low++) {
        print_stamp(*low);
    }
    /*for (set<Stamp>::iterator it = stamps.begin(); it != stamps.end(); it++) {
        if (get<0>(*it) >= begin_year && get<0>(*it) <= end_year) {
            print_stamp(*it);
        }
    }*/
}

bool is_correct_year(string word) {
    if (word.size() == 4 && isdigit(word[0]) && word[0] != zero
        && isdigit(word[1]) && isdigit(word[2]) && isdigit(word[3])) {
        return 1;
    }
    else {
        return 0;
    }
}

bool has_max_one_dot_or_comma(string word) {
    int dot_or_comma_met = 0;
	int s = word.size();
    for (int i = 1; i < s - 1; i++) {
        char character = word[i];

        if ((character == dot || character == comma) && dot_or_comma_met == 1) {
            return 0;
        }
        else if (dot_or_comma_met == 0) {
            dot_or_comma_met = 1;
        }
        else if (!isdigit(character)) {
            return 0;
        }
    }

    if (!isdigit(word[word.size() - 1])) {
        return 0;
    }

    return 1;
}

bool is_correct_price(string word) {
    if (isdigit(word[0]) && has_max_one_dot_or_comma(word) &&
        (word[0] != zero || (word.size() > 1 && (word[1] == dot || word[1] == comma)))) {
        return 1;
    }
    else {
        return 0;
    }
};




////////////////////////////////////////////////////////////////////////////////////////
Stamp parse_stamp_line(vector<string>& fragmented_line) {

    string post_office = empty_string;
    string stamp_year = empty_string;
    string stamp_name = empty_string;
    string stamp_price = empty_string;

    vector<string>::reverse_iterator it = fragmented_line.rbegin();

    // reading post office name
    for (; it != fragmented_line.rend(); it++) {
        if (is_correct_year(*it)) {
            break;
        }
        else if (post_office != empty_string) {
            post_office = *it + space + post_office;
        }
        else {
            post_office = *it;
        }
    }

    // reading rest if it is correct
    if (distance(it, fragmented_line.rend()) >= 3 && is_correct_year(*it)
        && is_correct_price(*(it+1)) && post_office != empty_string) {

        stamp_year = *it;

        it++;
        stamp_price = *it;

        it++;

        stamp_name = *it;
        it++;

        for (; it != fragmented_line.rend(); it++) {
            stamp_name = *it + space + stamp_name;
        }
    }

    return make_tuple(stamp_year, post_office, stamp_price, stamp_name);
}

Interval parse_interval_line(const vector<string>& fragmented_line) {
    string first_year = fragmented_line[0];
    string second_year = fragmented_line[1];

    if (is_correct_year(first_year) && is_correct_year(second_year)
        && first_year <= second_year) {
        return make_pair(first_year, second_year);
    }
    else {
        return make_pair(empty_string, empty_string);
    }
}

void parse_line(int line_count, string line, int* orders_started) {

    vector<string> fragmented_line;
    string word;
    stringstream line_stream = stringstream(line);

    while (line_stream >> word) {
        fragmented_line.push_back(word);
    }


    if (fragmented_line.size() == 2) { // it may be order
        Interval interval = parse_interval_line(fragmented_line);
        if (interval.first != empty_string) { // it is correct order
            print_stamp_interval(interval.first, interval.second);
            *orders_started = 1;
        }
        else {
            print_error_message(line_count, line);
        }
    }
    else if (fragmented_line.size() >= 4 && *orders_started == 0) { // it may be stamp
        Stamp stamp = parse_stamp_line(fragmented_line);
        if (get<0>(stamp) != empty_string) { // it is correct stamp
            stamps.insert(stamp);
        }
        else {
            print_error_message(line_count, line);
        }
    }
    else {
        print_error_message(line_count, line);
    }
}




int main() {

    string line;
    int line_count = 0;
    int orders_started = 0;

    while (getline(cin, line)) {
        line_count++;
        parse_line(line_count, line, &orders_started);
    }

    stamps.clear();

    return 0;
}
