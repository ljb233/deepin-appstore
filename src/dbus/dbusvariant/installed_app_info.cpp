/*
 * Copyright (C) 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dbus/dbusvariant/installed_app_info.h"

#include <QtDBus/QtDBus>

InstalledAppInfo::InstalledAppInfo() {

}

InstalledAppInfo::~InstalledAppInfo() {

}

void InstalledAppInfo::registerMetaType() {
  qRegisterMetaType<InstalledAppInfo>("InstalledAppInfo");
  qDBusRegisterMetaType<InstalledAppInfo>();
  qRegisterMetaType<InstalledAppInfoList>("InstalledAppInfoList");
  qDBusRegisterMetaType<InstalledAppInfoList>();
}

const QVariantMap InstalledAppInfo::toVariantMap() const {
  return QVariantMap {
      { "pkgName", this->pkg_name },
      { "version", this->version },
  };
}

QDebug operator<<(QDebug debug, const InstalledAppInfo& info) {
  debug << info.pkg_name
        << info.version;
  return debug;
}

QDBusArgument& operator<<(QDBusArgument& argument,
                          const InstalledAppInfo& info) {
  argument.beginStructure();
  argument << info.pkg_name
           << info.version;
  argument.endStructure();
  return argument;
}

QDataStream& operator<<(QDataStream& stream, const InstalledAppInfo& info) {
  stream << info.pkg_name
         << info.version;
  return stream;
}

const QDBusArgument& operator>>(const QDBusArgument& argument,
                                InstalledAppInfo& info) {
  argument.beginStructure();
  argument >> info.pkg_name
           >> info.version;
  argument.endStructure();
  return argument;
}

const QDataStream& operator>>(QDataStream& stream, InstalledAppInfo& info) {
  stream >> info.pkg_name
         >> info.version;
  return stream;
}
