#include "overseer-data.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>

DataIngestor::DataIngestor() {
}

DataIngestor::~DataIngestor() {
    clearData();
}

bool DataIngestor::loadFile(const QString& filePath) {
    clearData();

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << filePath << "Error:" << file.errorString();
        return false;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString content = in.readAll();
    file.close();

    if (!parseCSV(content)) {
        qWarning() << "Failed to parse CSV content from:" << filePath;
        return false;
    }

    qDebug() << "Loaded" << rows.size() << "rows from CSV file:" << filePath;
    return true;
}

void DataIngestor::clearData() {
    rows.clear();
    fieldNames.clear();
}

QStringList DataIngestor::getFieldValues(const QString& fieldName) const {
    QStringList values;

    for (const auto& row : rows) {
        if (row.fields.contains(fieldName)) {
            QString value = row.fields.value(fieldName).toString();
            if (!value.isEmpty() && !values.contains(value)) {
                values.append(value);
            }
        }
    }

    return values;
}

QList<IngestedDataRow> DataIngestor::findMatches(const QString& fieldName, const QString& searchText) const {
    QList<IngestedDataRow> matches;

    if (searchText.isEmpty()) {
        return matches;
    }

    QString searchLower = searchText.toLower();

    for (const auto& row : rows) {
        if (row.fields.contains(fieldName)) {
            QString fieldValue = row.fields.value(fieldName).toString().toLower();
            if (fieldValue.startsWith(searchLower)) {
                matches.append(row);
            }
        }
    }

    return matches;
}

IngestedDataRow DataIngestor::findFirstMatch(const QString& fieldName, const QString& searchText) const {
    auto matches = findMatches(fieldName, searchText);
    return matches.isEmpty() ? IngestedDataRow() : matches.first();
}

bool DataIngestor::parseCSV(const QString& content) {
    QStringList lines = content.split('\n', Qt::SkipEmptyParts);
    if (lines.isEmpty()) {
        qWarning() << "CSV content is empty";
        return false;
    }

    // Parse header row
    QString headerLine = lines.takeFirst();
    fieldNames = parseCSVLine(headerLine);

    if (fieldNames.isEmpty()) {
        qWarning() << "No field names found in CSV header";
        return false;
    }

    qDebug() << "Parsed field names:" << fieldNames;

    // Parse data rows
    for (const QString& line : lines) {
        if (line.trimmed().isEmpty()) {
            continue;
        }

        QStringList values = parseCSVLine(line);
        if (values.size() != fieldNames.size()) {
            qWarning() << "Row has" << values.size() << "values but expected" << fieldNames.size() << "fields";
            continue;
        }

        IngestedDataRow row;
        row.id = QString::number(rows.size() + 1); // Simple ID

        for (int i = 0; i < fieldNames.size(); ++i) {
            row.fields[fieldNames[i]] = values[i];
        }

        rows.append(row);
    }

    return !rows.isEmpty();
}

QStringList DataIngestor::parseCSVLine(const QString& line) const {
    QStringList fields;
    QString currentField;
    bool inQuotes = false;
    bool escapeNext = false;

    for (int i = 0; i < line.length(); ++i) {
        QChar c = line[i];

        if (escapeNext) {
            currentField += c;
            escapeNext = false;
            continue;
        }

        if (c == '\\') {
            escapeNext = true;
            continue;
        }

        if (c == '"') {
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                // Escaped quote
                currentField += '"';
                i++; // Skip next quote
            } else {
                inQuotes = !inQuotes;
            }
            continue;
        }

        if (c == ',' && !inQuotes) {
            fields.append(trimQuotes(currentField));
            currentField.clear();
            continue;
        }

        currentField += c;
    }

    // Add the last field
    fields.append(trimQuotes(currentField));

    return fields;
}

QString DataIngestor::trimQuotes(const QString& str) const {
    QString result = str.trimmed();
    if (result.startsWith('"') && result.endsWith('"')) {
        result = result.mid(1, result.length() - 2);
    }
    return result;
}
