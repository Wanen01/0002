/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "date_type.h"
#include "common/log/log.h"
#include "common/value.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <string>
#include <cstdlib>

using namespace std;

int DateType::compare(const Value &left, const Value &right) const
{
    ASSERT(left.attr_type() == AttrType::DATES, "left type is not date");
    ASSERT(right.attr_type() == AttrType::DATES, "right type is not date");

    if (left.date_year_ != right.date_year_) return left.date_year_ - right.date_year_;
    if (left.date_month_ != right.date_month_) return left.date_month_ - right.date_month_;
    return left.date_day_ - right.date_day_;
}

RC DateType::add(const Value &left, const Value &right, Value &result) const
{
    // 简单处理，加天数
    int total_days = left.date_day_ + right.date_day_;
    int day = total_days % 31; // 简单取余，不做复杂日历处理
    int month = left.date_month_ + right.date_month_ + total_days / 31;
    int year = left.date_year_ + right.date_year_ + month / 12;
    month = month % 12;
    if (day == 0) day = 1;
    if (month == 0) month = 1;
    result.set_year(year);
    result.set_month(month);
    result.set_day(day);
    return RC::SUCCESS;
}

RC DateType::subtract(const Value &left, const Value &right, Value &result) const
{
    // 简单处理，返回天数差
    int diff_days = (left.date_year_ - right.date_year_) * 365 +
                    (left.date_month_ - right.date_month_) * 30 +
                    (left.date_day_ - right.date_day_);
    result.set_int(diff_days);
    return RC::SUCCESS;
}

RC DateType::set_value_from_str(Value &val, const string &data) const
{
    if (data.empty()) {
        LOG_WARN("empty date string");
        return RC::GENERIC_ERROR;
    }

    int y = 0, m = 0, d = 0;
    if (sscanf(data.c_str(), "%4d-%2d-%2d", &y, &m, &d) != 3) {
        LOG_WARN("failed to parse date string: %s", data.c_str());
        return RC::GENERIC_ERROR;
    }

    val.set_type(AttrType::DATES);
    val.set_year(y);
    val.set_month(m);
    val.set_day(d);

    return RC::SUCCESS;
}

RC DateType::to_string(const Value &val, string &result) const
{
    stringstream ss;
    ss << setfill('0') << setw(4) << val.date_year_ << "-"
       << setw(2) << val.date_month_ << "-"
       << setw(2) << val.date_day_;
    result = ss.str();
    return RC::SUCCESS;
}
