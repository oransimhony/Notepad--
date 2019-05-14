#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter (parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
//    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
//                    << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
//                    << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
//                    << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
//                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
//                    << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
//                    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
//                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
//                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
//                    << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b"
//                    << "\\breturn\\b";

    keywordPatterns << "\\basm\\b" << "\\belse\\b" << "\\bnew\\b" << "\\bthis\\b" <<  "\\bauto\\b" << "\\benum\\b"
                    << "\\boperator\\b" << "\\bthrow bool\\b" << "\\bexplicit\\b" << "\\bprivate\\b" << "\\btrue\\b"
                    << "\\bbreak\\b" << "\\bexport\\b" << "\\bprotected\\b" << "\\btry\\b" << "\\bcase\\b" << "\\bextern\\b"
                    << "\\bpublic\\b" << "\\btypedef\\b" << "\\bcatch\\b" << "\\bfalse\\b" << "\\bregister\\b" << "\\btypeid\\b"
                    << "\\bchar\\b" << "\\bfloat\\b" << "\\breinterpret_cast\\b" << "\\btypename\\b" << "\\bclass\\b"
                    << "\\bfor\\b" << "\\breturn\\b" << "\\bunion\\b" << "\\bconst\\b" << "\\bfriend\\b" << "\\bshort\\b"
                    << "\\bunsigned\\b" << "\\bconst_cast\\b" << "\\bgoto\\b" << "\\bsigned\\b" << "\\busing\\b"
                    << "\\bcontinue\\b" << "\\bif\\b" << "\\bsizeof\\b" << "\\bvirtual\\b" << "\\bdefault\\b"
                    << "\\binline\\b" << "\\bstatic\\b" << "\\bvoid\\b" << "\\bdelete\\b" << "\\bint\\b"
                    << "\\bstatic_cast\\b" << "\\bvolatile\\b" << "\\bdo\\b" << "\\blong\\b" << "\\bstruct\\b"
                    << "\\bwchar_t\\b" << "\\bdouble\\b" << "\\bmutable\\b" << "\\bswitch\\b" << "\\bwhile\\b"
                    << "\\bdynamic_cast\\b" << "\\bnamespace\\b" << "\\btemplate\\b" << "\\band\\b" << "\\bbitor\\b"
                    << "\\bnot_eq\\b" << "\\bxor\\b" << "\\band_eq\\b" << "\\bcompl\\b" << "\\bor\\b" << "\\bxor_eq\\b"
                    << "\\bbitand\\b" << "\\bnot\\b" << "\\bor_eq\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::gray);
    singleLineCommentFormat.setFontItalic(true);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::gray);
    multiLineCommentFormat.setFontItalic(true);
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
