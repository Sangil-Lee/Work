<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns="http://www.w3.org/1999/xhtml" version="1.0">

  <xsl:import
    href="http://cdn.docbook.org/release/xsl/current/html/docbook.xsl" />
  <xsl:output method="html" indent="yes" encoding="utf-8"
    doctype-public="-//W3C//DTD HTML 4.01 Transitional//EN"
    doctype-system="http://www.w3.org/TR/html4/loose.dtd" />

  <xsl:param name="use.extensions" select="'0'" />
  <xsl:param name="html.stylesheet" select="'css/style.css'" />
  <xsl:param name="section.autolabel" select="'1'" />
  <xsl:param name="section.autolabel.max.depth" select="'2'" />
  <xsl:param name="section.label.includes.component.label" select="'1'" />
  <xsl:param name="toc.section.depth" select="'2'" />
  <xsl:param name="admon.graphics" select="'1'" />
  <xsl:param name="admon.graphics.path" select="'images/'" />

</xsl:stylesheet>
