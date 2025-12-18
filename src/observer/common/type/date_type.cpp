/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "common/lang/comparator.h"
#include "common/lang/sstream.h"
#include "common/log/log.h"
#include "common/type/date_type.h"
#include "common/value.h"

#include <iostream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <time.h>

int DateType::compare(const Value &left, const Value &right) const
{
    ASSERT(left.attr_type() == AttrType::DATES, "left type is not date");
    ASSERT(right.attr_type() == AttrType::DATES, "right type is not date");

    // 比较年月日
    if (left.date_year_ != right.date_year_) {
        return left.date_year_ - right.date_year_;
    }
    if (left.date_month_ != right.date_month_) {
        return left.date_month_ - right.date_month_;
    }
    return left.date_day_ - right.date_day_;
}

RC DateType::add(const Value &left, const Value &right, Value &result) const
{
    // 对日期类型，这里简单做整数加法
    result.set_int(left.get_int() + right.get_int());
    return RC::SUCCESS;
}

RC DateType::subtract(const Value &left, const Value &right, Value &result) const
{
    result.set_int(left.get_int() - right.get_int());
    return RC::SUCCESS;
}

RC DateType::set_value_from_str(Value &val, const string &data) const
{
    std::tm tm = {};
    std::istringstream iss(data);

    // 尝试按完整时间解析
    iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (iss.fail()) {
        // 尝试只解析日期部分
        iss.clear();
        iss.str(data);
        iss >> std::get_time(&tm, "%Y-%m-%d");
        if (iss.fail()) {
            LOG_WARN("failed to parse date string: %s", data.c_str());
            return RC::SCHEMA_FIELD_TYPE_MISMATCH;
        }
    }

    // 解析成功，设置 Value
    val.set_type(AttrType::DATES);
    val.set_year(tm.tm_year + 1900);   // tm_year 从 1900 开始
    val.set_month(tm.tm_mon + 1);      // tm_mon 0-11
    val.set_day(tm.tm_mday);

    return RC::SUCCESS;
}

RC DateType::to_string(const Value &val, string &result) const
{
    std::ostringstream ss;
    ss << val.date_year_ << "-"
       << (val.date_month_ < 10 ? "0" : "") << val.date_month_ << "-"
       << (val.date_day_ < 10 ? "0" : "") << val.date_day_;
    result = ss.str();
    return RC::SUCCESS;
}
