#ifndef OVERSEER_DATA_H
#define OVERSEER_DATA_H

#include "overseer-types.h"
#include <QString>
#include <QList>
#include <QMap>

// DataIngestor handles CSV file loading and auto-fill functionality
class DataIngestor {
public:
    DataIngestor();
    ~DataIngestor();

    // File operations
    bool loadFile(const QString& filePath);
    void clearData();

    // Data access
    const QList<IngestedDataRow>& getRows() const { return rows; }
    QStringList getFieldNames() const { return fieldNames; }
    QStringList getFieldValues(const QString& fieldName) const;

    // Auto-fill functionality
    QList<IngestedDataRow> findMatches(const QString& fieldName, const QString& searchText) const;
    IngestedDataRow findFirstMatch(const QString& fieldName, const QString& searchText) const;

    // Utility
    bool hasData() const { return !rows.isEmpty(); }
    int getRowCount() const { return rows.size(); }

private:
    QList<IngestedDataRow> rows;
    QStringList fieldNames;

    // CSV parsing
    bool parseCSV(const QString& content);
    QStringList parseCSVLine(const QString& line) const;
    QString trimQuotes(const QString& str) const;
};

#endif // OVERSEER_DATA_H
