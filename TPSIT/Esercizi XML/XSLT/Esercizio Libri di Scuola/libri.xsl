<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:template match="/">
        <html lang="en">
        <head>
            <meta charset="UTF-8"/>
            <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
            <title>Libri di testo</title>

            <link rel="stylesheet" href="css/stileHTML.css"/>
        </head>
        <body>
            <h1>Benvenuto!</h1>
            <table>
                <tr>
                    <th class="b-dx">Materia</th>
                    <th class="b-dx">Codice</th>
                    <th class="b-dx">Autore</th>
                    <th class="b-dx">Titolo</th>
                    <th class="b-dx">Editore</th>
                    <th class="b-dx">Prezzo</th>
                    <th>Copertina</th>
                </tr>
                <xsl:for-each select="//libro">
                    <tr>
                        <td class="b-dx"><xsl:value-of select="materia"/></td>
                        <td class="b-dx"><xsl:value-of select="codice"/></td>
                        <td class="b-dx"><xsl:value-of select="autore"/></td>
                        <td class="b-dx"><xsl:value-of select="titolo"/></td>
                        <td class="b-dx"><xsl:value-of select="editore"/></td>
                        <td class="b-dx"><xsl:value-of select="prezzo"/>€</td>
                        <td>
                            <img style="width: 100%">
                                <xsl:attribute name="src">img/<xsl:value-of select="immagine"/></xsl:attribute>
                            </img>
                        </td>
                    </tr>
                </xsl:for-each>
            </table>
        </body>
        </html>
    </xsl:template>
</xsl:stylesheet>