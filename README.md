Ez a szakdolgozat projektemhez készült Arduino program kód.

Ez egy távcső vezérlő (GOTO) mechanika.

Ezt egy ESP32 modulra töltöttem fel.
Az arduino-hoz kapcsoltam még RTC-t SD-kártya modult, egy optikai csatlakozót (ezzel vezérlem a DSLR fényképezőgépet), két LED-et és egy nyomógombot.
Illetve az akkumulátor feszültség mérésére pedig két ellenállást használtam és UART csatlakozáson keresztül kapcsolódott a másik arduino panelhez, ami a motort vezérli (Teensy 4.0).

A program lényege, hogy vezérelje a motor vezérlőt, illetve kommunikáljon Bluetooth-on keresztül az Androidkos alkalmazással.
Az alkalmazástól meg kapja a beállítani kívánt objektum katalógus kódját és ki keresi az SD-kártyáról az objektumot majd a koordinátáit.
Ezt követően a jelenlegi pozícióját ismérvé a távcsőnek ki számítja, hogy az új pozícióhoz mennyit kell mennie melyik irányba. Majd ezt konkrét lépés számra lefordítva ismérvé a felbontást az áttéteken.
Majd ezt elküldi a motor vezérlő egységnek UART-on.

