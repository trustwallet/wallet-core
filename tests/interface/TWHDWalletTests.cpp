// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

#include "Coin.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWData.h>
#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWMnemonic.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWBase58.h>
#include <TrustWalletCore/TWCoinType.h>
#include <proto/Stellar.pb.h>

#include "HexCoding.h"
#include "Data.h"

#include <gtest/gtest.h>
#include <thread>
#include <vector>

const auto wordsStr = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
const auto words = STRING(wordsStr);
const auto passphrase = STRING("TREZOR");
const auto seedHex = "7ae6f661157bda6492f6162701e570097fc726b6235011ea5ad09bf04986731ed4d92bc43cbdee047b60ea0dd1b1fa4274377c9bf5bd14ab1982c272d8076f29";
const auto entropyHex = "ba5821e8c356c05ba5f025d9532fe0f21f65d594";


inline void assertSeedEq(const std::shared_ptr<TWHDWallet>& wallet, const char* expected) {
    const auto seed = WRAPD(TWHDWalletSeed(wallet.get()));
    assertHexEqual(seed, expected);
}

inline void assertMnemonicEq(const std::shared_ptr<TWHDWallet>& wallet, const char* expected) {
    const auto mnemonic = WRAPS(TWHDWalletMnemonic(wallet.get()));
    assertStringsEqual(mnemonic, expected);
}

inline void assertEntropyEq(const std::shared_ptr<TWHDWallet>& wallet, const char* expected) {
    const auto entropy = WRAPD(TWHDWalletEntropy(wallet.get()));
    assertHexEqual(entropy, expected);
}

TEST(HDWallet, CreateFromMnemonic) {
    const auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    assertMnemonicEq(wallet, wordsStr);
    assertEntropyEq(wallet, entropyHex);
    assertSeedEq(wallet, seedHex);
}

TEST(HDWallet, CreateFromEntropy) {
    const auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithEntropy(DATA(entropyHex).get(), passphrase.get()));
    assertMnemonicEq(wallet, wordsStr);
    assertSeedEq(wallet, seedHex);
    assertEntropyEq(wallet, entropyHex);
}

TEST(HDWallet, Generate) {
    const auto wallet = WRAP(TWHDWallet, TWHDWalletCreate(128, passphrase.get()));
    EXPECT_TRUE(TWMnemonicIsValid(WRAPS(TWHDWalletMnemonic(wallet.get())).get()));
}

// Spanish dictionary, without accents  https://github.com/bitcoin/bips/blob/master/bip-0039/spanish.txt
const char* SpanishBip39Dictionary =
   "abaco abdomen abeja abierto abogado abono aborto abrazo abrir abuelo abuso acabar academia acceso accion aceite acelga acento "
   "aceptar acido aclarar acne acoger acoso activo acto actriz actuar acudir acuerdo acusar adicto admitir adoptar adorno aduana "
   "adulto aereo afectar aficion afinar afirmar agil agitar agonia agosto agotar agregar agrio agua agudo aguila aguja ahogo "
   "ahorro aire aislar ajedrez ajeno ajuste alacran alambre alarma alba album alcalde aldea alegre alejar alerta aleta alfiler "
   "alga algodon aliado aliento alivio alma almeja almibar altar alteza altivo alto altura alumno alzar amable amante amapola "
   "amargo amasar ambar ambito ameno amigo amistad amor amparo amplio ancho anciano ancla andar anden anemia angulo anillo animo "
   "anis anotar antena antiguo antojo anual anular anuncio anadir anejo ano apagar aparato apetito apio aplicar apodo aporte "
   "apoyo aprender aprobar apuesta apuro arado arana arar arbitro arbol arbusto archivo arco arder ardilla arduo area arido "
   "aries armonia arnes aroma arpa arpon arreglo arroz arruga arte artista asa asado asalto ascenso asegurar aseo asesor asiento "
   "asilo asistir asno asombro aspero astilla astro astuto asumir asunto atajo ataque atar atento ateo atico atleta atomo atraer "
   "atroz atun audaz audio auge aula aumento ausente autor aval avance avaro ave avellana avena avestruz avion aviso ayer ayuda "
   "ayuno azafran azar azote azucar azufre azul baba babor bache bahia baile bajar balanza balcon balde bambu banco banda bano "
   "barba barco barniz barro bascula baston basura batalla bateria batir batuta baul bazar bebe bebida bello besar beso bestia "
   "bicho bien bingo blanco bloque blusa boa bobina bobo boca bocina boda bodega boina bola bolero bolsa bomba bondad bonito "
   "bono bonsai borde borrar bosque bote botin boveda bozal bravo brazo brecha breve brillo brinco brisa broca broma bronce "
   "brote bruja brusco bruto buceo bucle bueno buey bufanda bufon buho buitre bulto burbuja burla burro buscar butaca buzon "
   "caballo cabeza cabina cabra cacao cadaver cadena caer cafe caida caiman caja cajon cal calamar calcio caldo calidad calle "
   "calma calor calvo cama cambio camello camino campo cancer candil canela canguro canica canto cana canon caoba caos capaz "
   "capitan capote captar capucha cara carbon carcel careta carga carino carne carpeta carro carta casa casco casero caspa castor "
   "catorce catre caudal causa cazo cebolla ceder cedro celda celebre celoso celula cemento ceniza centro cerca cerdo cereza "
   "cero cerrar certeza cesped cetro chacal chaleco champu chancla chapa charla chico chiste chivo choque choza chuleta chupar "
   "ciclon ciego cielo cien cierto cifra cigarro cima cinco cine cinta cipres circo ciruela cisne cita ciudad clamor clan claro "
   "clase clave cliente clima clinica cobre coccion cochino cocina coco codigo codo cofre coger cohete cojin cojo cola colcha "
   "colegio colgar colina collar colmo columna combate comer comida comodo compra conde conejo conga conocer consejo contar "
   "copa copia corazon corbata corcho cordon corona correr coser cosmos costa craneo crater crear crecer creido crema cria crimen "
   "cripta crisis cromo cronica croqueta crudo cruz cuadro cuarto cuatro cubo cubrir cuchara cuello cuento cuerda cuesta cueva "
   "cuidar culebra culpa culto cumbre cumplir cuna cuneta cuota cupon cupula curar curioso curso curva cutis dama danza dar "
   "dardo datil deber debil decada decir dedo defensa definir dejar delfin delgado delito demora denso dental deporte derecho "
   "derrota desayuno deseo desfile desnudo destino desvio detalle detener deuda dia diablo diadema diamante diana diario dibujo "
   "dictar diente dieta diez dificil digno dilema diluir dinero directo dirigir disco diseno disfraz diva divino doble doce "
   "dolor domingo don donar dorado dormir dorso dos dosis dragon droga ducha duda duelo dueno dulce duo duque durar dureza duro "
   "ebano ebrio echar eco ecuador edad edicion edificio editor educar efecto eficaz eje ejemplo elefante elegir elemento elevar "
   "elipse elite elixir elogio eludir embudo emitir emocion empate empeno empleo empresa enano encargo enchufe encia enemigo "
   "enero enfado enfermo engano enigma enlace enorme enredo ensayo ensenar entero entrar envase envio epoca equipo erizo escala "
   "escena escolar escribir escudo esencia esfera esfuerzo espada espejo espia esposa espuma esqui estar este estilo estufa "
   "etapa eterno etica etnia evadir evaluar evento evitar exacto examen exceso excusa exento exigir exilio existir exito experto "
   "explicar exponer extremo fabrica fabula fachada facil factor faena faja falda fallo falso faltar fama familia famoso faraon "
   "farmacia farol farsa fase fatiga fauna favor fax febrero fecha feliz feo feria feroz fertil fervor festin fiable fianza "
   "fiar fibra ficcion ficha fideo fiebre fiel fiera fiesta figura fijar fijo fila filete filial filtro fin finca fingir finito "
   "firma flaco flauta flecha flor flota fluir flujo fluor fobia foca fogata fogon folio folleto fondo forma forro fortuna forzar "
   "fosa foto fracaso fragil franja frase fraude freir freno fresa frio frito fruta fuego fuente fuerza fuga fumar funcion funda "
   "furgon furia fusil futbol futuro gacela gafas gaita gajo gala galeria gallo gamba ganar gancho ganga ganso garaje garza "
   "gasolina gastar gato gavilan gemelo gemir gen genero genio gente geranio gerente germen gesto gigante gimnasio girar giro "
   "glaciar globo gloria gol golfo goloso golpe goma gordo gorila gorra gota goteo gozar grada grafico grano grasa gratis grave "
   "grieta grillo gripe gris grito grosor grua grueso grumo grupo guante guapo guardia guerra guia guino guion guiso guitarra "
   "gusano gustar haber habil hablar hacer hacha hada hallar hamaca harina haz hazana hebilla hebra hecho helado helio hembra "
   "herir hermano heroe hervir hielo hierro higado higiene hijo himno historia hocico hogar hoguera hoja hombre hongo honor "
   "honra hora hormiga horno hostil hoyo hueco huelga huerta hueso huevo huida huir humano humedo humilde humo hundir huracan "
   "hurto icono ideal idioma idolo iglesia iglu igual ilegal ilusion imagen iman imitar impar imperio imponer impulso incapaz "
   "indice inerte infiel informe ingenio inicio inmenso inmune innato insecto instante interes intimo intuir inutil invierno "
   "ira iris ironia isla islote jabali jabon jamon jarabe jardin jarra jaula jazmin jefe jeringa jinete jornada joroba joven "
   "joya juerga jueves juez jugador jugo juguete juicio junco jungla junio juntar jupiter jurar justo juvenil juzgar kilo koala "
   "labio lacio lacra lado ladron lagarto lagrima laguna laico lamer lamina lampara lana lancha langosta lanza lapiz largo larva "
   "lastima lata latex latir laurel lavar lazo leal leccion leche lector leer legion legumbre lejano lengua lento lena leon "
   "leopardo lesion letal letra leve leyenda libertad libro licor lider lidiar lienzo liga ligero lima limite limon limpio lince "
   "lindo linea lingote lino linterna liquido liso lista litera litio litro llaga llama llanto llave llegar llenar llevar llorar "
   "llover lluvia lobo locion loco locura logica logro lombriz lomo lonja lote lucha lucir lugar lujo luna lunes lupa lustro "
   "luto luz maceta macho madera madre maduro maestro mafia magia mago maiz maldad maleta malla malo mama mambo mamut manco "
   "mando manejar manga maniqui manjar mano manso manta manana mapa maquina mar marco marea marfil margen marido marmol marron "
   "martes marzo masa mascara masivo matar materia matiz matriz maximo mayor mazorca mecha medalla medio medula mejilla mejor "
   "melena melon memoria menor mensaje mente menu mercado merengue merito mes meson meta meter metodo metro mezcla miedo miel "
   "miembro miga mil milagro militar millon mimo mina minero minimo minuto miope mirar misa miseria misil mismo mitad mito mochila "
   "mocion moda modelo moho mojar molde moler molino momento momia monarca moneda monja monto mono morada morder moreno morir "
   "morro morsa mortal mosca mostrar motivo mover movil mozo mucho mudar mueble muela muerte muestra mugre mujer mula muleta "
   "multa mundo muneca mural muro musculo museo musgo musica muslo nacar nacion nadar naipe naranja nariz narrar nasal natal "
   "nativo natural nausea naval nave navidad necio nectar negar negocio negro neon nervio neto neutro nevar nevera nicho nido "
   "niebla nieto ninez nino nitido nivel nobleza noche nomina noria norma norte nota noticia novato novela novio nube nuca nucleo "
   "nudillo nudo nuera nueve nuez nulo numero nutria oasis obeso obispo objeto obra obrero observar obtener obvio oca ocaso "
   "oceano ochenta ocho ocio ocre octavo octubre oculto ocupar ocurrir odiar odio odisea oeste ofensa oferta oficio ofrecer "
   "ogro oido oir ojo ola oleada olfato olivo olla olmo olor olvido ombligo onda onza opaco opcion opera opinar oponer optar "
   "optica opuesto oracion orador oral orbita orca orden oreja organo orgia orgullo oriente origen orilla oro orquesta oruga "
   "osadia oscuro osezno oso ostra otono otro oveja ovulo oxido oxigeno oyente ozono pacto padre paella pagina pago pais pajaro "
   "palabra palco paleta palido palma paloma palpar pan panal panico pantera panuelo papa papel papilla paquete parar parcela "
   "pared parir paro parpado parque parrafo parte pasar paseo pasion paso pasta pata patio patria pausa pauta pavo payaso peaton "
   "pecado pecera pecho pedal pedir pegar peine pelar peldano pelea peligro pellejo pelo peluca pena pensar penon peon peor "
   "pepino pequeno pera percha perder pereza perfil perico perla permiso perro persona pesa pesca pesimo pestana petalo petroleo "
   "pez pezuna picar pichon pie piedra pierna pieza pijama pilar piloto pimienta pino pintor pinza pina piojo pipa pirata pisar "
   "piscina piso pista piton pizca placa plan plata playa plaza pleito pleno plomo pluma plural pobre poco poder podio poema "
   "poesia poeta polen policia pollo polvo pomada pomelo pomo pompa poner porcion portal posada poseer posible poste potencia "
   "potro pozo prado precoz pregunta premio prensa preso previo primo principe prision privar proa probar proceso producto proeza "
   "profesor programa prole promesa pronto propio proximo prueba publico puchero pudor pueblo puerta puesto pulga pulir pulmon "
   "pulpo pulso puma punto punal puno pupa pupila pure quedar queja quemar querer queso quieto quimica quince quitar rabano "
   "rabia rabo racion radical raiz rama rampa rancho rango rapaz rapido rapto rasgo raspa rato rayo raza razon reaccion realidad "
   "rebano rebote recaer receta rechazo recoger recreo recto recurso red redondo reducir reflejo reforma refran refugio regalo "
   "regir regla regreso rehen reino reir reja relato relevo relieve relleno reloj remar remedio remo rencor rendir renta reparto "
   "repetir reposo reptil res rescate resina respeto resto resumen retiro retorno retrato reunir reves revista rey rezar rico "
   "riego rienda riesgo rifa rigido rigor rincon rinon rio riqueza risa ritmo rito rizo roble roce rociar rodar rodeo rodilla "
   "roer rojizo rojo romero romper ron ronco ronda ropa ropero rosa rosca rostro rotar rubi rubor rudo rueda rugir ruido ruina "
   "ruleta rulo rumbo rumor ruptura ruta rutina sabado saber sabio sable sacar sagaz sagrado sala saldo salero salir salmon "
   "salon salsa salto salud salvar samba sancion sandia sanear sangre sanidad sano santo sapo saque sardina sarten sastre satan "
   "sauna saxofon seccion seco secreto secta sed seguir seis sello selva semana semilla senda sensor senal senor separar sepia "
   "sequia ser serie sermon servir sesenta sesion seta setenta severo sexo sexto sidra siesta siete siglo signo silaba silbar "
   "silencio silla simbolo simio sirena sistema sitio situar sobre socio sodio sol solapa soldado soledad solido soltar solucion "
   "sombra sondeo sonido sonoro sonrisa sopa soplar soporte sordo sorpresa sorteo sosten sotano suave subir suceso sudor suegra "
   "suelo sueno suerte sufrir sujeto sultan sumar superar suplir suponer supremo sur surco sureno surgir susto sutil tabaco "
   "tabique tabla tabu taco tacto tajo talar talco talento talla talon tamano tambor tango tanque tapa tapete tapia tapon taquilla "
   "tarde tarea tarifa tarjeta tarot tarro tarta tatuaje tauro taza tazon teatro techo tecla tecnica tejado tejer tejido tela "
   "telefono tema temor templo tenaz tender tener tenis tenso teoria terapia terco termino ternura terror tesis tesoro testigo "
   "tetera texto tez tibio tiburon tiempo tienda tierra tieso tigre tijera tilde timbre timido timo tinta tio tipico tipo tira "
   "tiron titan titere titulo tiza toalla tobillo tocar tocino todo toga toldo tomar tono tonto topar tope toque torax torero "
   "tormenta torneo toro torpedo torre torso tortuga tos tosco toser toxico trabajo tractor traer trafico trago traje tramo "
   "trance trato trauma trazar trebol tregua treinta tren trepar tres tribu trigo tripa triste triunfo trofeo trompa tronco "
   "tropa trote trozo truco trueno trufa tuberia tubo tuerto tumba tumor tunel tunica turbina turismo turno tutor ubicar ulcera "
   "umbral unidad unir universo uno untar una urbano urbe urgente urna usar usuario util utopia uva vaca vacio vacuna vagar "
   "vago vaina vajilla vale valido valle valor valvula vampiro vara variar varon vaso vecino vector vehiculo veinte vejez vela "
   "velero veloz vena vencer venda veneno vengar venir venta venus ver verano verbo verde vereda verja verso verter via viaje "
   "vibrar vicio victima vida video vidrio viejo viernes vigor vil villa vinagre vino vinedo violin viral virgo virtud visor "
   "vispera vista vitamina viudo vivaz vivero vivir vivo volcan volumen volver voraz votar voto voz vuelo vulgar yacer yate "
   "yegua yema yerno yeso yodo yoga yogur zafiro zanja zapato zarza zona zorro zumo zurdo";

const std::vector<std::string> splitWords(const std::string& wordList) {
    std::vector<std::string> list;
    auto l = wordList.length();
    std::string currentWord;
    for (auto i = 0; i < l; ++i) {
        if (wordList[i] == ' ') {
            if (currentWord.length() > 2) {
                list.push_back(currentWord);
                currentWord = "";
            }
        } else {
            currentWord += wordList[i];
        }
    }
    if (currentWord.length() > 2) {
        list.push_back(currentWord);
    }
    return list;
}

int findWordInWordlist(const std::string& word, const std::vector<std::string>& list) {
    for (auto i = 0; i < list.size(); ++i) {
        if (list[i] == word) {
            return i;
        }
    }
    return -1;
}

TEST(HDWallet, CreateFromMnemonicUnchecked) {
    // Non-English is not supported, workaround is to do validation and entropy bit generation beforehand, here is an example
    
    // prepare word list
    const auto wordList = splitWords(SpanishBip39Dictionary);
    ASSERT_EQ(wordList.size(), 2048);

    // find word indices, concatenate to entropy
    const auto spanishMnemonic = "llanto radical atraer riesgo actuar masa fondo cielo dieta archivo sonrisa mamut";
    const auto mnemonic = splitWords(spanishMnemonic);
    ASSERT_EQ(mnemonic.size(), 12);
    TW::Data entropy(17); // (12 * 11) / 8 + 1
    int bi = 0;
    for (auto i = 0; i < mnemonic.size(); ++i) {
        auto idx = findWordInWordlist(mnemonic[i], wordList);
        ASSERT_TRUE(idx >= 0);
        for (auto ki = 0; ki < 11; ki++) {
            if (idx & (1 << (10 - ki))) {
                entropy[bi / 8] |= 1 << (7 - (bi % 8));
            }
            ++bi;
        }
    }
    TW::Data entropy2 = TW::data(entropy.data(), 16); // truncate
    EXPECT_EQ(TW::hex(entropy2), "81d8185b64f0371457198b43c22b6c43");

    const auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonicUnchecked(STRING(spanishMnemonic).get(), STRING("").get(), WRAPD(TWDataCreateWithBytes(entropy2.data(), entropy2.size())).get()));
    assertMnemonicEq(wallet, spanishMnemonic);
    assertEntropyEq(wallet, "81d8185b64f0371457198b43c22b6c43");
    assertSeedEq(wallet, "ec8f8703432fc7d32e699ee056e9d84b1435e6a64a6a40ad63dbde11eab189a276ddcec20f3326d3c6ee39cbd018585b104fc3633b801c011063ae4c318fb9b6");
}

TEST(HDWallet, SeedWithExtraSpaces) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    assertSeedEq(wallet, "7ae6f661157bda6492f6162701e570097fc726b6235011ea5ad09bf04986731ed4d92bc43cbdee047b60ea0dd1b1fa4274377c9bf5bd14ab1982c272d8076f29");
}

TEST(HDWallet, CreateFromMnemonicNoPassword) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), STRING("").get()));
    assertSeedEq(wallet, "354c22aedb9a37407adc61f657a6f00d10ed125efa360215f36c6919abd94d6dbc193a5f9c495e21ee74118661e327e84a5f5f11fa373ec33b80897d4697557d");
}

TEST(HDWallet, CreateFromStrengthInvalid) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreate(64, STRING("").get()));
    ASSERT_EQ(wallet.get(), nullptr);
}

TEST(HDWallet, CreateFromMnemonicInvalid) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(STRING("THIS IS INVALID MNEMONIC").get(), STRING("").get()));
    ASSERT_EQ(wallet.get(), nullptr);
}

TEST(HDWallet, MasterPrivateKey) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), STRING("").get()));
    auto key1 = WRAP(TWPrivateKey, TWHDWalletGetMasterKey(wallet.get(), TWCurveSECP256k1));
    auto hexKey1 = WRAPD(TWPrivateKeyData(key1.get()));

    auto key2 = WRAP(TWPrivateKey,TWHDWalletGetMasterKey(wallet.get(), TWCurveED25519));
    auto hexKey2 = WRAPD(TWPrivateKeyData(key2.get()));

    assertHexEqual(hexKey1, "d1b2b553b053f278d510a8494ead811252b1d5ec0da4434d0997a75de92bcea9");
    assertHexEqual(hexKey2, "d258c2521f7802b8e83c32f2cc97bd06b69747847390c5e247a3d19faa74202e");
}

TEST(HDWallet, Derive) {
    const auto derivationPath = TW::derivationPath(TWCoinTypeEthereum);

    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key0 = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeEthereum));

    auto publicKey0 = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key0.get(), false));
    auto publicKey0Data = WRAPD(TWPublicKeyData(publicKey0.get()));

    assertHexEqual(publicKey0Data, "0414acbe5a06c68210fcbb77763f9612e45a526990aeb69d692d705f276f558a5ae68268e9389bb099ed5ac84d8d6861110f63644f6e5b447e3f86b4bab5dee011");
}

TEST(HDWallet, DeriveBitcoinNonextended) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeBitcoin));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key.get(), false));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));

    assertHexEqual(publicKeyData, "047ea5dff03f677502c4a1d73c5ac897200e56b155e876774c8fba0cc22f80b9414ec07cda7b1c9a84c2e04ea2746c21afacc5e91b47427c453c3f1a4a3e983ce5");
    // Note: address derivation does not work with nonextended public key
}

TEST(HDWallet, DeriveBitcoinExtended) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeBitcoin));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key.get(), true));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));

    assertHexEqual(publicKeyData, "037ea5dff03f677502c4a1d73c5ac897200e56b155e876774c8fba0cc22f80b941");

    auto address = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeBitcoin, publicKey.get()));
    assertStringsEqual(address, "bc1qumwjg8danv2vm29lp5swdux4r60ezptzz7ce85");
}

TEST(HDWallet, DeriveAddressBitcoin) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto address = WRAP(TWString, TWHDWalletGetAddressForCoin(wallet.get(), TWCoinTypeBitcoin));
    assertStringsEqual(address, "bc1qumwjg8danv2vm29lp5swdux4r60ezptzz7ce85");
}

TEST(HDWallet, DeriveEthereum) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeEthereum));
    auto key2 = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeSmartChain));

    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key.get(), false));
    auto publicKey2 = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key2.get(), false));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));

    auto address = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeEthereum, publicKey.get()));
    auto address2 = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeSmartChain, publicKey2.get()));

    assertHexEqual(publicKeyData, "0414acbe5a06c68210fcbb77763f9612e45a526990aeb69d692d705f276f558a5ae68268e9389bb099ed5ac84d8d6861110f63644f6e5b447e3f86b4bab5dee011");
    assertStringsEqual(address, "0x27Ef5cDBe01777D62438AfFeb695e33fC2335979");
    assertStringsEqual(address2, "0x27Ef5cDBe01777D62438AfFeb695e33fC2335979");
}

TEST(HDWallet, DeriveAddressEthereum) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto address = WRAP(TWString, TWHDWalletGetAddressForCoin(wallet.get(), TWCoinTypeEthereum));
    assertStringsEqual(address, "0x27Ef5cDBe01777D62438AfFeb695e33fC2335979");
}

TEST(HDWallet, DeriveCosmos) {
    // use `gaiacli keys add key_name` to generate mnemonic words and private key
    auto words = STRING("attract term foster morning tail foam excite copper disease measure cheese camera rug enroll cause flip sword waste try local purchase between idea thank");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), STRING("").get()));

    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeCosmos));
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));

    assertHexEqual(privateKeyData, "80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");

    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(privateKey.get(), true));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));

    assertHexEqual(publicKeyData, "0257286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc5");
}

TEST(HDWallet, DeriveNimiq) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeNimiq));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519(key.get()));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));

    assertHexEqual(publicKeyData, "1937063865fe3294ccf3017837207bb3fea71a53720ae631b77bf9d5ca4f7f4c");
}

TEST(HDWallet, DeriveTezos) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeTezos));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519(key.get()));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));

    assertHexEqual(publicKeyData, "c834147f97bcf95bf01f234455646a197f70b25e93089591ffde8122370ad371");
}

TEST(HDWallet, DeriveDoge) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeDogecoin));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key.get(), true));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));

    assertHexEqual(publicKeyData, "039fc134623a37c8ee44881f088a599cc44ba8a95f91f860b99d9d3b11f487e4c0");

    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeDogecoin, TWHDVersionDGPV));
    assertStringsEqual(xprv, "dgpv57ru95KiYUB5oWm2CVQH4heh1f7E9dNGdRHHVThcQkLeQ2HHxVJfFYefnpKrWZ6L6EDKJHUVq4Yyd5kPZKnRePfkCz3EzkySTydXKbgjcxN");

    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeDogecoin, TWHDVersionDGUB));
    assertStringsEqual(xpub, "dgub8rjvUmFc6cqR6NRBEj2FBZCHUDUrykPyv24Vea6bCsPex5PzNFrRtr4KN37XgwuVzzC2MikJRW2Ddcp99Ehsqp2iaU4eerNCJVruKxz6Gci");
}

TEST(HDWallet, DeriveZilliqa) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeZilliqa));
    auto publicKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeySecp256k1(key.get(), true));
    auto publicKeyData = WRAPD(TWPublicKeyData(publicKey.get()));

    assertHexEqual(publicKeyData, "0262746d4988c63b9972c63272461e9fa080d4dfa2a1fda3dd01285620c0a60c22");
}

TEST(HDWallet, DeriveAionPrivateKey) {
    auto words = STRING("zero behind diesel afraid edge salad drop episode high pear twin resource");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), STRING("").get()));
    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeAion));
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));
    assertHexEqual(privateKeyData, "db33ffdf82c7ba903daf68d961d3c23c20471a8ce6b408e52d579fd8add80cc9");
}

TEST(HDWallet, DeriveFIO) {
    auto words = STRING("valley alien library bread worry brother bundle hammer loyal barely dune brave");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), STRING("").get()));
    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeFIO));
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));

    uint8_t bytes[] = {0x80};
    auto prefix = WRAPD(TWDataCreateWithBytes(bytes, 1));
    TWDataAppendData(prefix.get(), privateKeyData.get());
    auto base58 = WRAPS(TWBase58Encode(prefix.get()));
    
    assertHexEqual(privateKeyData, "ea8eb60b7e5868e218f248e032769020b4fea5dcfd02f2992861eaf4fb534854");
    assertStringsEqual(base58, "5Kbb37EAqQgZ9vWUHoPiC2uXYhyGSFNbL6oiDp24Ea1ADxV1qnu");
}

TEST(HDWallet, DeriveAlgorand) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeAlgorand));
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));
    auto address = WRAPS(TWCoinTypeDeriveAddress(TWCoinTypeAlgorand, privateKey.get()));

    assertHexEqual(privateKeyData, "ce0b7ac644e2b7d9d14d3928b11643f43e48c33d3e328d059fef8add7f070e82");
}

TEST(HDWallet, DeriveElrond) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeElrond));
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));
    auto address = WRAPS(TWCoinTypeDeriveAddress(TWCoinTypeElrond, privateKey.get()));

    assertHexEqual(privateKeyData, "0eb593141de897d60a0883320793eb49e63d556ccdf783a87ec014f150d50453");
    assertStringsEqual(address, "erd1a6l7q9cfvrgr80xuzm37tapdr4zm3mwrtl6vt8f45df45x7eadfs8ds5vv");
}

TEST(HDWallet, DeriveBinance) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeBinance));
    auto key2 = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeSmartChain));
    auto keyData = WRAPD(TWPrivateKeyData(key.get()));
    auto keyData2 = WRAPD(TWPrivateKeyData(key2.get()));

    auto expected = "ca81b1b0974aa063de2f74c17b9dc364a8208d105659f4f900c121fb170922fe";
    auto expectedETH = "c4f77b4a9f5a0db3a7ffc3599e61bef986037ae9a7cc1972a10d55c030270020";

    assertHexEqual(keyData, expected);
    assertHexEqual(keyData2, expectedETH);
}

TEST(HDWallet, DeriveAvalancheCChain) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    auto key = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeAvalancheCChain));
    auto keyData = WRAPD(TWPrivateKeyData(key.get()));

    auto expectedETH = "c4f77b4a9f5a0db3a7ffc3599e61bef986037ae9a7cc1972a10d55c030270020";

    assertHexEqual(keyData, expectedETH);
}

TEST(HDWallet, ExtendedKeys) {
    auto words = STRING("abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), STRING("").get()));

    auto xprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeBitcoin, TWHDVersionXPRV));
    auto xpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeBitcoin, TWHDVersionXPUB));

    assertStringsEqual(xprv, "xprv9xpXFhFpqdQK3TmytPBqXtGSwS3DLjojFhTGht8gwAAii8py5X6pxeBnQ6ehJiyJ6nDjWGJfZ95WxByFXVkDxHXrqu53WCRGypk2ttuqncb");
    assertStringsEqual(xpub, "xpub6BosfCnifzxcFwrSzQiqu2DBVTshkCXacvNsWGYJVVhhawA7d4R5WSWGFNbi8Aw6ZRc1brxMyWMzG3DSSSSoekkudhUd9yLb6qx39T9nMdj");

    auto yprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP49, TWCoinTypeBitcoin, TWHDVersionYPRV));
    auto ypub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP49, TWCoinTypeBitcoin, TWHDVersionYPUB));
    assertStringsEqual(yprv, "yprvAHwhK6RbpuS3dgCYHM5jc2ZvEKd7Bi61u9FVhYMpgMSuZS613T1xxQeKTffhrHY79hZ5PsskBjcc6C2V7DrnsMsNaGDaWev3GLRQRgV7hxF");
    assertStringsEqual(ypub, "ypub6Ww3ibxVfGzLrAH1PNcjyAWenMTbbAosGNB6VvmSEgytSER9azLDWCxoJwW7Ke7icmizBMXrzBx9979FfaHxHcrArf3zbeJJJUZPf663zsP");

    auto zprv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP84, TWCoinTypeBitcoin, TWHDVersionZPRV));
    auto zpub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP84, TWCoinTypeBitcoin, TWHDVersionZPUB));
    assertStringsEqual(zprv, "zprvAdG4iTXWBoARxkkzNpNh8r6Qag3irQB8PzEMkAFeTRXxHpbF9z4QgEvBRmfvqWvGp42t42nvgGpNgYSJA9iefm1yYNZKEm7z6qUWCroSQnE");
    assertStringsEqual(zpub, "zpub6rFR7y4Q2AijBEqTUquhVz398htDFrtymD9xYYfG1m4wAcvPhXNfE3EfH1r1ADqtfSdVCToUG868RvUUkgDKf31mGDtKsAYz2oz2AGutZYs");
    
    auto emptyPrv = WRAPS(TWHDWalletGetExtendedPrivateKey(wallet.get(), TWPurposeBIP44, TWCoinTypeEthereum, TWHDVersionNone));
    auto emptyPub = WRAPS(TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeEthereum, TWHDVersionNone));
    assertStringsEqual(emptyPrv, "");
    assertStringsEqual(emptyPub, "");
}

TEST(HDWallet, PublicKeyFromX) {
    auto xpub = STRING("xpub6BosfCnifzxcFwrSzQiqu2DBVTshkCXacvNsWGYJVVhhawA7d4R5WSWGFNbi8Aw6ZRc1brxMyWMzG3DSSSSoekkudhUd9yLb6qx39T9nMdj");
    auto xpubAddr2 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeBitcoinCash, STRING("m/44'/145'/0'/0/2").get()));
    auto xpubAddr9 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeBitcoinCash, STRING("m/44'/145'/0'/0/9").get()));

    auto data2 = WRAPD(TWPublicKeyData(xpubAddr2.get()));
    auto data9 = WRAPD(TWPublicKeyData(xpubAddr9.get()));

    assertHexEqual(data2, "0338994349b3a804c44bbec55c2824443ebb9e475dfdad14f4b1a01a97d42751b3");
    assertHexEqual(data9, "03786c1d274f2c804ff9a57d8e7289c281d4aef15e17187ad9f9c3722d81a6ae66");
}

TEST(HDWallet, PublicKeyInvalid) {
    auto xpub = STRING("xpub0000");
    auto xpubAddr = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeBitcoinCash, STRING("m/44'/145'/0'/0/0").get()));
    ASSERT_EQ(xpubAddr.get(), nullptr);
}

TEST(HDWallet, PublicKeyFromY) {
    auto ypub = STRING("ypub6Ww3ibxVfGzLrAH1PNcjyAWenMTbbAosGNB6VvmSEgytSER9azLDWCxoJwW7Ke7icmizBMXrzBx9979FfaHxHcrArf3zbeJJJUZPf663zsP");
    auto ypubAddr3 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(ypub.get(), TWCoinTypeBitcoin, STRING("m/44'/0'/0'/0/3").get()));
    auto ypubAddr10 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(ypub.get(), TWCoinTypeBitcoin, STRING("m/44'/0'/0'/0/10").get()));

    auto data3 = WRAPD(TWPublicKeyData(ypubAddr3.get()));
    auto data10 = WRAPD(TWPublicKeyData(ypubAddr10.get()));

    assertHexEqual(data3, "0299bd0bdc081a9888fac95a33e8bebcdeeb57cf7477f2f0721362f3a51a157227");
    assertHexEqual(data10, "03a39ad9c0d19bb43c45643582614298c96b0f7c9462c0de789c69013b0d609d1c");
}

TEST(HDWallet, PublicKeyFromZ) {
    auto zpub = STRING("zpub6rFR7y4Q2AijBEqTUquhVz398htDFrtymD9xYYfG1m4wAcvPhXNfE3EfH1r1ADqtfSdVCToUG868RvUUkgDKf31mGDtKsAYz2oz2AGutZYs");
    auto zpubAddr4 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(zpub.get(), TWCoinTypeBitcoin, STRING("m/44'/0'/0'/0/4").get()));
    auto zpubAddr11 = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(zpub.get(), TWCoinTypeBitcoin, STRING("m/44'/0'/0'/0/11").get()));

    auto data4 = WRAPD(TWPublicKeyData(zpubAddr4.get()));
    auto data11 = WRAPD(TWPublicKeyData(zpubAddr11.get()));

    auto address4 = WRAPS(TWCoinTypeDeriveAddressFromPublicKey(TWCoinTypeBitcoin, zpubAddr4.get()));

    assertHexEqual(data4, "03995137c8eb3b223c904259e9b571a8939a0ec99b0717684c3936407ca8538c1b");
    assertHexEqual(data11, "0226a07edd0227fa6bc36239c0bd4db83d5e488f8fb1eeb68f89a5be916aad2d60");

    assertStringsEqual(address4, "bc1qm97vqzgj934vnaq9s53ynkyf9dgr05rargr04n");
}

TEST(HDWallet, PublicKeyFromExtended_Ethereum) {
    const auto xpub = STRING("xpub6C7LtZJgtz1BKXG9mExKUxYvX7HSF38UMMmGbpqNQw3DfYwAw8E6sH7VSVxFipvEEm2afSqTjoRgcLmycXX4zfxCWJ4HY73a9KdgvfHEQGB");
    const auto xpubAddr = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeEthereum, STRING("m/44'/60'/0'/0/1").get()));
    ASSERT_NE(xpubAddr.get(), nullptr);
    auto data = WRAPD(TWPublicKeyData(xpubAddr.get()));
    ASSERT_NE(data.get(), nullptr);
    assertHexEqual(data, "044516c4aa5352035e1bb5be132694e1389a4ac37d32e5e717d35ee4c4dfab513226a9d14ea37a55962ad3644a08e2ce551b4495beabb9b09e688c7b92eba18acc");
}

TEST(HDWallet, PublicKeyFromExtended_NIST256p1) {
    const auto xpub = STRING("xpub6BosfCnifzxcFwrSzQiqu2DBVTshkCXacvNsWGYJVVhhawA7d4R5WSWGFNbi8Aw6ZRc1brxMyWMzG3DSSSSoekkudhUd9yLb6qx39T9nMdj");
    const auto xpubAddr = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeNEO, STRING("m/44'/888'/0'/0/0").get())); // Neo
    ASSERT_NE(xpubAddr.get(), nullptr);
    auto data = WRAPD(TWPublicKeyData(xpubAddr.get()));
    ASSERT_NE(data.get(), nullptr);
    assertHexEqual(data, "03774c910fcf07fa96886ea794f0d5caed9afe30b44b83f7e213bb92930e7df4bd");
}

TEST(HDWallet, PublicKeyFromExtended_Negative) {
    const auto xpub = STRING("xpub6BosfCnifzxcFwrSzQiqu2DBVTshkCXacvNsWGYJVVhhawA7d4R5WSWGFNbi8Aw6ZRc1brxMyWMzG3DSSSSoekkudhUd9yLb6qx39T9nMdj");
    {   // Ed25519
        const auto xpubAddr = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeSolana, STRING("m/44'/501'/0'").get())); // Solana
        EXPECT_EQ(xpubAddr.get(), nullptr);
    }
    {   // Ed25519Extended
        const auto xpubAddr = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeCardano, STRING("m/1852'/1815'/0'/0/0").get())); // Cardano
        EXPECT_EQ(xpubAddr.get(), nullptr);
    }
    {   // Ed25519Blake2bNano
        const auto xpubAddr = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeNano, STRING("m/44'/165'/0'").get())); // Nano
        EXPECT_EQ(xpubAddr.get(), nullptr);
    }
    {   // Curve25519
        const auto xpubAddr = WRAP(TWPublicKey, TWHDWalletGetPublicKeyFromExtended(xpub.get(), TWCoinTypeWaves, STRING("m/44'/5741564'/0'/0'/0'").get())); // Waves
        EXPECT_EQ(xpubAddr.get(), nullptr);
    }
}

TEST(HDWallet, MultipleThreads) {
    auto passphrase = STRING("");

    auto f = [&passphrase](int n) {
        std::vector<TWString*> keys;
        for (int i = 0; i < n; i++) {
            auto wallet = WRAP(TWHDWallet, TWHDWalletCreate(128, passphrase.get()));
            TWString* key = TWHDWalletGetExtendedPublicKey(wallet.get(), TWPurposeBIP44, TWCoinTypeEthereum, TWHDVersionNone);
            keys.push_back(key);
        }
        // delete returned strings
        for (int i = n-1; i >= 0; i--) {
            TWStringDelete(keys[i]);
        }
    };

    // Ensure multiple threads cause no asserts
    std::thread th1(f, 10);
    std::thread th2(f, 10);
    std::thread th3(f, 10);

    th1.join();
    th2.join();
    th3.join();
}

TEST(HDWallet, GetDerivedKey) {
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(words.get(), passphrase.get()));
    const auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetDerivedKey(wallet.get(), TWCoinTypeBitcoin, 0, 0, 0));
    const auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));
    assertHexEqual(privateKeyData, "1901b5994f075af71397f65bd68a9fff8d3025d65f5a2c731cf90f5e259d6aac");
}
