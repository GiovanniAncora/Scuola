<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  
  <xsl:template match="/">
    <html>
    <body>
      <div style="display: flex;">
        <div style="width: 50%;">
          <xsl:apply-templates select="document('articoli.xml')/articoli"/>
        </div>
        <div style="width: 50%;">
          <xsl:apply-templates select="document('pubblicita.xml')/pubblicita"/>
        </div>
      </div>
    </body>
    </html>
  </xsl:template>
  
  <xsl:template match="articoli">
    <h2>Articoli</h2>
    <xsl:for-each select="articolo">
      <h4><xsl:value-of select="titolo"/></h4>
      <p><xsl:value-of select="paragrafo"/></p>
    </xsl:for-each>
  </xsl:template>
  
  <xsl:template match="pubblicita">
    <h2>Pubblicità</h2>
    <xsl:for-each select="pubbl">
      <h4><xsl:value-of select="titolo"/></h4>
      <p><xsl:value-of select="contenuto"/></p>
    </xsl:for-each>
  </xsl:template>

</xsl:stylesheet>