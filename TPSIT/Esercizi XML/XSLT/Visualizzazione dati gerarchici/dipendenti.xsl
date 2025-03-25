<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:template match="/">
    <html>
    <body>
      <h1>Struttura dipendenti</h1>
      <xsl:apply-templates select="dipendenti/dipendente"/>
    </body>
    </html>
  </xsl:template>

  <xsl:template match="dipendente">
    <li>
      <xsl:value-of select="nome"/>
      <ul>
        <xsl:for-each select="sottoposti">
          <xsl:apply-templates select="dipendente"/>
        </xsl:for-each>
      </ul>
    </li>
  </xsl:template>

</xsl:stylesheet>