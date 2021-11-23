
#include <fstream>
#include <future>

#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <tuple>
#include <string_view>

#include <utility>
#include <type_traits>
#include "types.h"
#include "display.h"

#include <iostream>


bool is_indentation(char const c) {
	return (c == ' ') | (c == '\t');
}


template<typename T>
bool is_indented(T const text) {
	return is_indentation(text[0]);
}

int leading_indent(std::string_view const text) {
	int indent_count = 0;
	for(const auto& c : text) {
		if(is_indentation(c))
			indent_count++;
		else
			break;
	}
	return indent_count;
}

int get_day(std::string_view const line) {
	if(line.find("pon") != line.npos) return 0; // is default
	if(line.find("tor") != line.npos) return 1;
	if(line.find("sre") != line.npos) return 2;
	if(line.find("čet") != line.npos ||
	   line.find("cet") != line.npos) return 3;
	if(line.find("pet") != line.npos) return 4;
	if(line.find("sob") != line.npos) return 5;
	if(line.find("ned") != line.npos) return 6;
	throw std::runtime_error{std::string("failed to parse day: ") + std::string(line)};
}
std::tuple<int, int> get_start_end(std::string_view line) {
	int head, i = -1;                                   // tail is un-needed, i = seek head
	while(is_indentation(line[++i]));                  // skip to first relevant character
	head = i;                                          // store it (and move to the next)
	std::string_view start_hr( line.data()+head  , 2); // military time
	std::string_view start_min(line.data()+head+2, 2);
	i += 4;                                            // 4 characters parsed
	while(line[i++] != '-');                           // seek to seperator
	head = i;                                          // next character should be part of the next time
	std::string_view end_hr( line.data()+head  , 2);   // military time
	std::string_view end_min(line.data()+head+2, 2);

	// gave up on googling, gonna implement it myself
	auto const digit = [](char const c) {return c - '0';};
	int const start = 60*(10*digit(start_hr[0] ) + digit(start_hr[1] )) +
	                      10*digit(start_min[0]) + digit(start_min[1]);
	int const end   = 60*(10*digit(end_hr[0]   ) + digit(end_hr[1]   )) +
	                      10*digit(end_min[0]  ) + digit(end_min[1]  );

	return {start,end};
}

std::vector<Period> parse_subject(
               std::vector<std::string_view> const lines,
						   std::pair<int,int> const bounds) noexcept {
	if(bounds.first >= bounds.second)
		return {};
	try {
		std::vector<Period> ret;
		ret.reserve(bounds.second-bounds.first-1);

		int day = -1;
		// skip the first line (subject)
		for(int i = bounds.first+1; i != bounds.second; i++) {
			auto const& line = lines[i];
			switch(leading_indent(line)) {
				case 1:
					day = get_day(line);
					break;
				case 2:
					auto const& [start, end] = get_start_end(line);
					ret.emplace_back(Period{day, start, end, std::string(lines[bounds.first])});
					break;
			}
		}
		return ret;
	} catch (std::exception const& e) {
		debug_print((std::string("parse_subject: ") + std::string(e.what())).c_str());
	}
	return {};
}

std::vector<std::string_view> split_lines(std::string const& text) {
	std::vector<std::string_view> lines;
	lines.reserve(1024);

	int head = 0, tail;
	auto len = [&head, &tail]() -> int {return tail-head;};
	auto beg = [&head, ptr = text.data()]() {return ptr+head;};
	while(true) {
		tail = text.find('\n', head);
		if(static_cast<std::size_t>(tail) == text.npos)
			break;
		lines.emplace_back(beg(), len());
		head = tail+1;
	}
	lines.emplace_back(beg(), text.length()-head);

	lines.shrink_to_fit();
	return lines;
}

std::vector<std::pair<int, int>> get_subject_lines(std::vector<std::string_view> const& lines) {
	std::vector<std::pair<int, int>> subject_lines;
	subject_lines.reserve(1024);

	int head = -1, tail = -1;
	for(int i = 0; i < static_cast<int>(lines.size()); i++) {
		if(lines[i].length() == 0)
			continue;
		if(!is_indented(lines[i]) && lines[i][0] != '#') { // no indentation = subject declaration
			tail = i-1;
			if(tail != -1)
				subject_lines.push_back({head, tail});
			head = i;
		}
	}
	subject_lines.push_back({subject_lines.back().second+1, lines.size()-1});
	subject_lines.shrink_to_fit();
	return subject_lines;
}

std::vector<Period> parse_text(std::string const filename) {

	std::ifstream                    file(filename,std::ios::in);   // source file
	if(!file.good()) {
		// todo: setup this
		throw;
	}
	std::string const                text((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>())); // text storage in memory
	file.~basic_ifstream();                                         // no longer needed

	// basic string processing
	std::vector<std::string_view> const lines = split_lines(text);                    // lines of text
	std::vector<std::pair<int, int>> const subject_lines = get_subject_lines(lines);  // first and one-after-last lines of one subject

	using Parse_subject_type = decltype(parse_subject(std::vector<std::string_view>{}, std::pair<int, int>{}));
	// dispatch async jobs
	std::vector<
		std::future<
			Parse_subject_type>> jobs(subject_lines.size());
	for(auto const& subject : subject_lines) {
		jobs.push_back(std::async(std::launch::async, parse_subject, lines, subject));
	}

	// here, parsing is done in parrallel

	// collect the jobs
	Parse_subject_type periods; // a reasonable estimate?
	periods.reserve(1024*1024);
	for(auto& job : jobs) {
		if(!job.valid())
			continue;
		for(auto& period : job.get()) {
			try {
				periods.push_back(period);
			} catch(std::exception const& e) {
				debug_print(e.what());
				break;
			}
		}
	}
	periods.shrink_to_fit();           // save memory
	return periods;
}

int main() {
	auto const periods = parse_text("sched.txt");
	return 0;
}