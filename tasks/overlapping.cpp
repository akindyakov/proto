/**
* Число переговорок
* > C++
* > Алгоритмы/структуры данных
* Заданы интервалы встреч [s1,e1],[s2,e2]. Встречи могут накладываться друг на друга. Необходимо посчитать минимальное число переговорок, чтобы провести все встречи.
* Что хотим услышать
* стек. Число вложенных скобок ()((())).
* Если встречи корректно описаны, можно и без стека - просто считать текущее количество открытых.
*/

#include <algorithm>
#include <iostream>
#include <queue>

struct Meeting {
    Meeting(
        int begin_
        , int end_
    )
        : begin(begin_)
        , end(end_)
    {
    }

    int begin;
    int end;
};

auto finMaxOverlapping(const std::vector<Meeting>& meetings_) {
    auto meetings = meetings_;
    auto meetingBeginLess = [](
        const Meeting& first
        , const Meeting& second
    ) {
        return first.begin < second.begin;
    };
    auto meetingEndLess = [](
        const Meeting& first
        , const Meeting& second
    ) {
        return first.end > second.end;
    };
    std::sort(meetings.begin(), meetings.end(), meetingBeginLess);
    auto inProgress = std::priority_queue<
        Meeting,
        std::vector<Meeting>,
        decltype(meetingEndLess)
    >(
        meetingEndLess
    );
    auto maxSize = inProgress.size();
    for (const auto& next : meetings) {
        while (!inProgress.empty() && inProgress.top().end <= next.begin) {
            inProgress.pop();
        }
        inProgress.push(next);
        if (maxSize < inProgress.size()) {
            maxSize = inProgress.size();
        }
    }
    return maxSize;
}

template<typename F, typename S>
void testEqual(int n, F f, S s) {
    if (f == s) {
        std::cout << n << ": pass\n";
    } else {
        std::cout << n << ": failed " << f << " is not equal to " << s << "\n";
    }
}

int main() {
    testEqual(0, 1, finMaxOverlapping({{0, 1}, {1, 2}}));
    testEqual(1, 2, finMaxOverlapping({{0, 2}, {0, 1}, {1, 2}}));
    testEqual(2, 3, finMaxOverlapping({{1, 20}, {0, 2}, {0, 1}, {1, 2}}));
}
