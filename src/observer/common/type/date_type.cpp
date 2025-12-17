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
  if (right.attr_type() == AttrType::DATES) {
    return common::compare_int((void *)&left.value_.int_value_, (void *)&right.value_.int_value_);
  } else if (right.attr_type() == AttrType::FLOATS) {
    return common::compare_float((void *)&left.value_.int_value_, (void *)&right.value_.int_value_);
  }
  return INT32_MAX;
}

RC DateType::add(const Value &left, const Value &right, Value &result) const
{
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
  RC                rc = RC::SUCCESS;

  std::tm tm = {};
  std::istringstream iss(data);
  iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

  if (!iss.fail()) {
    rc = RC::SCHEMA_FIELD_TYPE_MISMATCH;
  } else {
    std::time_t tt = std::mktime(&tm);
    struct tm *p = localtime(&tt);
    val.set_type(AttrType::DATES);
    val.set_year(p->tm_year);
    val.set_month(p->tm_mon);
    val.set_day(p->tm_mday);
  }
  return rc;
}

RC DateType::to_string(const Value &val, string &result) const
{
  stringstream ss;
  ss << val.date_year_ << "-" << val.date_month_ << "-" << val.date_day_;
  result = ss.str();
  return RC::SUCCESS;
}