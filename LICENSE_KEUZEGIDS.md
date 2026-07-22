# Licentiekeuzegids

## Doel

Deze keuzegids documenteert de afweging die aan de definitieve licentiekeuze voorafging. Zij blijft behouden als historisch referentiedocument en is geen juridisch advies.

## Vastgelegde filosofie

Het framework moet breed bruikbaar zijn, terwijl verbeteringen aan de kern zo veel mogelijk terugvloeien naar de community.

## Te beoordelen licentierichtingen

### GNU GPL v3

Sterke wederkerigheid. Verspreide afgeleide software moet in beginsel onder dezelfde licentie beschikbaar blijven. Dit beschermt openheid sterk, maar kan integratie in gesloten toepassingen beperken.

### GNU LGPL v3

Wederkerigheid voor de library zelf, met ruimere mogelijkheden om de library vanuit andere software te gebruiken. Juridisch moet worden nagegaan hoe statische koppeling binnen Arduino-builds wordt behandeld.

### Mozilla Public License 2.0

Wederkerigheid op bestandsniveau. Gewijzigde MPL-bestanden blijven open, terwijl combinatie met andere code mogelijk blijft. De praktische geschiktheid voor een Arduino-library moet juridisch worden gecontroleerd.

## Afzonderlijke onderdelen

Er moet expliciet worden beslist of dezelfde of verschillende licenties gelden voor:

- broncode;
- voorbeelden;
- Markdown-documentatie;
- elektronische schema's;
- PCB- en CAD-bronbestanden;
- afbeeldingen en meetgegevens.

## Niet automatisch passend

Permissieve licenties zoals MIT of Apache 2.0 verplichten niet dat verbeteringen terugvloeien. Zij passen daarom niet vanzelfsprekend bij de huidige projectfilosofie.

## Juridisch beoordeelde aandachtspunten

- geldt wederkerigheid voor de volledige toepassing of alleen voor frameworkbestanden;
- welke verplichtingen gelden bij commercieel gebruik;
- hoe bijdragen juridisch worden aanvaard;
- hoe auteursrechtvermeldingen worden behouden;
- welke licentie compatibel is met de gebruikte externe libraries;
- hoe hardwareontwerpen en documentatie worden gelicentieerd.

## Definitieve keuze

**Definitieve keuze: GNU LGPL v3.0-or-later.**

De officiële licentietekst staat in [`LICENSE`](LICENSE). De projectspecifieke toelichting staat in [`LICENSE.md`](LICENSE.md).
