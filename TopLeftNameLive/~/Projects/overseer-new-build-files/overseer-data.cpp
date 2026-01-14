#include "overseer-data.h"

#include <QFile>

#include <QTextStream>



bool DataIngestor::loadFile(const QString& filePath) {

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QTextStream stream(&file);

    QString content = stream.readAll();

    file.close();

    QString format = detectFormat(filePath);

    return (format == "csv") ? parseCSV(content) : parsePlainText(content);

}



bool DataIngestor::parseCSV(const QString& content) {

    clear();

    QStringList lines = content.split('\n', Qt::SkipEmptyParts);

    if (lines.isEmpty()) return false;

    columns_ = lines[0].split(',', Qt::SkipEmptyParts);

    for (auto& col : columns_) col = col.trimmed();

    for (int i = 1; i < lines.size(); ++i) {

        QStringList values = lines[i].split(',', Qt::SkipEmptyParts);

        IngestedDataRow row;

        for (int j = 0; j < columns_.size() && j < values.size(); ++j) {

            row.fields[columns_[j]] = values[j].trimmed();

        }

        if (!row.fields.isEmpty()) data_.append(row);

    }

    return !data_.isEmpty();

}



bool DataIngestor::parsePlainText(const QString& content) {

    clear();

    QStringList lines = content.split('\n', Qt::SkipEmptyParts);

    if (lines.isEmpty()) return false;

    columns_.append("Item");

    for (const auto& line : lines) {

        if (line.trimmed().isEmpty()) continue;

        IngestedDataRow row;

        row.fields["Item"] = line.trimmed();

        data_.append(row);

    }

    return !data_.isEmpty();

}



QString DataIngestor::detectFormat(const QString& filePath) {

    return filePath.endsWith(".csv", Qt::CaseInsensitive) ? "csv" : "txt";

}



IngestedDataRow* DataIngestor::findMatch(const QString& fieldName, const QString& searchText) {

    if (searchText.isEmpty() || data_.isEmpty()) return nullptr;

    QString searchLower = searchText.toLower();

    for (auto& row : data_) {

        if (row.fields.contains(fieldName)) {

            QString value = row.fields[fieldName].toLower();

            if (value.startsWith(searchLower)) return &row;

        }

    }

    return nullptr;

}



QStringList DataIngestor::getFieldValues(const QString& fieldName) const {

    QStringList values;

    for (const auto& row : data_) {

        if (row.fields.contains(fieldName)) values.append(row.fields[fieldName]);

    }

    return values;

}



void DataIngestor::clear() { data_.clear(); columns_.clear(); }
