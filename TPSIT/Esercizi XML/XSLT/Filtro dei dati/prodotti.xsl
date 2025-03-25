<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="/">
    <html>
    <body>
      <h1>Prodotti</h1>
      <h4>Ma solo i più costosi!</h4>

      <table border="1">
        <tr>
          <xsl:for-each select="/prodotti/prodotto[1]/*">
            <th><xsl:value-of select="name()"/></th>
          </xsl:for-each>
        </tr>

        <xsl:for-each select="/prodotti/prodotto[prezzo > 2]">
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
</xsl:transform>