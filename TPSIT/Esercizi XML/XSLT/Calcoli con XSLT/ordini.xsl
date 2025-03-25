<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="/">
    <h1>Elenco Ordini</h1>
    <table>
      <tr>
        <th>Prodotto</th>
        <th>Quantità</th>
        <th>Prezzo</th>
        <th>Totale</th>
      </tr>
      <xsl:for-each select="ordini/ordine">
        <tr>
          <td><xsl:value-of select="prodotto"/></td>
          <td><xsl:value-of select="quantita"/></td>
          <td><xsl:value-of select="prezzoUnitario"/></td>
          <td><xsl:value-of select="quantita * prezzoUnitario"/></td>
        </tr>
      </xsl:for-each>
    </table>
  </xsl:template>
</xsl:stylesheet>