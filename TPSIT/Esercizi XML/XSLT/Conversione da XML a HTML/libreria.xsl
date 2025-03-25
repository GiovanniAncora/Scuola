<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="/">
    <html>
    <body>
      <h1>Libreria</h1>
      <table border="1">
        <tr>
          <xsl:for-each select="/libreria/libro[1]/*">
            <th><xsl:value-of select="name()"/></th>
          </xsl:for-each>
        </tr>
        <xsl:for-each select="/libreria/libro">
          <tr>
            <xsl:for-each select="*">
              <td><xsl:value-of select="."/></td>
            </xsl:for-each>
          </tr>
        </xsl:for-each>
      </table>
    </body>
    </html>
  </xsl:template>
</xsl:stylesheet>