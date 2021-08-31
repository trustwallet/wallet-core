// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HDWallet.h"
#include "Mnemonic.h"
#include "Bitcoin/Address.h"
#include "Bitcoin/CashAddress.h"
#include "Bitcoin/SegwitAddress.h"
#include "Ethereum/Address.h"
#include "HexCoding.h"
#include "PublicKey.h"
#include "Hash.h"
#include "Base58.h"
#include "Coin.h"
#include "interface/TWTestUtilities.h"

#include <gtest/gtest.h>

namespace TW {

const auto mnemonic1 = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
const auto passphrase = "passphrase";

TEST(HDWallet, generate) {
    {
        HDWallet wallet = HDWallet(128, passphrase);
        EXPECT_TRUE(Mnemonic::isValid(wallet.getMnemonic()));
        EXPECT_EQ(wallet.getPassphrase(), passphrase);
        EXPECT_EQ(wallet.getEntropy().size(), 16);
    }
    {
        HDWallet wallet = HDWallet(256, passphrase);
        EXPECT_TRUE(Mnemonic::isValid(wallet.getMnemonic()));
        EXPECT_EQ(wallet.getPassphrase(), passphrase);
        EXPECT_EQ(wallet.getEntropy().size(), 33);
    }
}

TEST(HDWallet, generateInvalid) {
    EXPECT_EXCEPTION(HDWallet(64, passphrase), "Invalid strength");
    EXPECT_EXCEPTION(HDWallet(129, passphrase), "Invalid strength");
    EXPECT_EXCEPTION(HDWallet(512, passphrase), "Invalid strength");
}

TEST(HDWallet, createFromMnemonic) {
    {
        HDWallet wallet = HDWallet(mnemonic1, passphrase);
        EXPECT_EQ(wallet.getMnemonic(), mnemonic1);
        EXPECT_EQ(wallet.getPassphrase(), passphrase);
        EXPECT_EQ(hex(wallet.getEntropy()), "ba5821e8c356c05ba5f025d9532fe0f21f65d594");
        EXPECT_EQ(hex(wallet.getSeed()), "143cd5fc27ae46eb423efebc41610473f5e24a80f2ca2e2fa7bf167e537f58f4c68310ae487fce82e25bad29bab2530cf77fd724a5ebfc05a45872773d7ee2d6");
    }
    {   // empty passphrase
        HDWallet wallet = HDWallet(mnemonic1, "");
        EXPECT_EQ(wallet.getMnemonic(), mnemonic1);
        EXPECT_EQ(wallet.getPassphrase(), "");
        EXPECT_EQ(hex(wallet.getEntropy()), "ba5821e8c356c05ba5f025d9532fe0f21f65d594");
        EXPECT_EQ(hex(wallet.getSeed()), "354c22aedb9a37407adc61f657a6f00d10ed125efa360215f36c6919abd94d6dbc193a5f9c495e21ee74118661e327e84a5f5f11fa373ec33b80897d4697557d");
    }
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

TEST(HDWallet, createFromSpanishMnemonicDict) {
    auto dictionary = Bip39Dictionary::prepareDictionary(SpanishBip39Dictionary);
    ASSERT_TRUE(dictionary.first);
    ASSERT_TRUE(dictionary.second.pointers() != nullptr);
    {
        HDWallet wallet = HDWallet("llanto radical atraer riesgo actuar masa fondo cielo dieta archivo sonrisa mamut", "", dictionary.second);
        EXPECT_EQ(wallet.getMnemonic(), "llanto radical atraer riesgo actuar masa fondo cielo dieta archivo sonrisa mamut");
        EXPECT_EQ(hex(wallet.getEntropy()), "81d8185b64f0371457198b43c22b6c43");
        EXPECT_EQ(hex(wallet.getSeed()), "ec8f8703432fc7d32e699ee056e9d84b1435e6a64a6a40ad63dbde11eab189a276ddcec20f3326d3c6ee39cbd018585b104fc3633b801c011063ae4c318fb9b6");
    }
    {
        HDWallet wallet = HDWallet("careta llanto jefe tarjeta tren osadia carga alejar banda recurso aguila macho", "", dictionary.second);
        EXPECT_EQ(wallet.getMnemonic(), "careta llanto jefe tarjeta tren osadia carga alejar banda recurso aguila macho");
        EXPECT_EQ(hex(wallet.getEntropy()), "2b3039d7f0fee54d0ad0441bb87c19c2");
        EXPECT_EQ(hex(wallet.getSeed()), "47b6780dbb64c5ad8285b19c43c3781de97bc0b3aaec62edd81c831d2d1e05044daf9f03a2d618765296c381942ebdd9787ba08c3d23d020d5fc21d9f2cb980e");
    }
}

TEST(HDWallet, createFromMnemonicInvalid) {
    EXPECT_EXCEPTION(HDWallet("THIS IS AN INVALID MNEMONIC", passphrase), "Invalid mnemonic");
    EXPECT_EXCEPTION(HDWallet("", passphrase), "Invalid mnemonic");
}

TEST(HDWallet, createFromEntropy) {
    {
        HDWallet wallet = HDWallet(parse_hex("ba5821e8c356c05ba5f025d9532fe0f21f65d594"), passphrase);
        EXPECT_EQ(wallet.getMnemonic(), mnemonic1);
    }
}

TEST(HDWallet, createFromEntropyInvalid) {
    EXPECT_EXCEPTION(HDWallet(parse_hex(""), passphrase), "Invalid mnemonic data");
    EXPECT_EXCEPTION(HDWallet(parse_hex("123456"), passphrase), "Invalid mnemonic data");
}

TEST(HDWallet, recreateFromEntropy) {
    {
        HDWallet wallet1 = HDWallet(mnemonic1, passphrase);
        EXPECT_EQ(wallet1.getMnemonic(), mnemonic1);
        EXPECT_EQ(hex(wallet1.getEntropy()), "ba5821e8c356c05ba5f025d9532fe0f21f65d594");
        HDWallet wallet2 = HDWallet(wallet1.getEntropy(), passphrase);
        EXPECT_EQ(wallet2.getMnemonic(), wallet1.getMnemonic());
        EXPECT_EQ(wallet2.getEntropy(), wallet1.getEntropy());
        EXPECT_EQ(wallet2.getSeed(), wallet1.getSeed());
    }
}

TEST(HDWallet, privateKeyFromXPRV) {
    const std::string xprv = "xprv9yqEgpMG2KCjvotCxaiMkzmKJpDXz2xZi3yUe4XsURvo9DUbPySW1qRbdeDLiSxZt88hESHUhm2AAe2EqfWM9ucdQzH3xv1HoKoLDqHMK9n";
    auto privateKey = HDWallet::getPrivateKeyFromExtended(xprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 3));
    ASSERT_TRUE(privateKey);
    auto publicKey = privateKey->getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Bitcoin::CashAddress(publicKey);

    EXPECT_EQ(hex(publicKey.bytes), "025108168f7e5aad52f7381c18d8f880744dbee21dc02c15abe512da0b1cca7e2f");
    EXPECT_EQ(address.string(), "bitcoincash:qp3y0dyg6ya8nt4n3algazn073egswkytqs00z7rz4");
}

TEST(HDWallet, privateKeyFromXPRV_Invalid) {
    const std::string xprv = "xprv9y0000";
    auto privateKey = HDWallet::getPrivateKeyFromExtended(xprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 3));
    ASSERT_FALSE(privateKey);
}

TEST(HDWallet, privateKeyFromXPRV_InvalidVersion) {
    {
        // Version bytes (first 4) are invalid, 0x00000000
        const std::string xprv = "11117pE7xwz2GARukXY8Vj2ge4ozfX4HLgy5ztnJXjr5btzJE8EbtPhZwrcPWAodW2aFeYiXkXjSxJYm5QrnhSKFXDgACcFdMqGns9VLqESCq3";
        auto privateKey = HDWallet::getPrivateKeyFromExtended(xprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 3));
        ASSERT_FALSE(privateKey);
    }
    {
        // Version bytes (first 4) are invalid, 0xdeadbeef
        const std::string xprv = "pGoh3VZXR4mTkT4bfqj4paog12KmHkAWkdLY8HNsZagD1ihVccygLr1ioLBhVQsny47uEh5swP3KScFc4JJrazx1Y7xvzmH2y5AseLgVMwomBTg2";
        auto privateKey = HDWallet::getPrivateKeyFromExtended(xprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 3));
        ASSERT_FALSE(privateKey);
    }
}

TEST(HDWallet, privateKeyFromExtended_InvalidCurve) {
    // invalid coin & curve, should fail
    const std::string xprv = "xprv9yqEgpMG2KCjvotCxaiMkzmKJpDXz2xZi3yUe4XsURvo9DUbPySW1qRbdeDLiSxZt88hESHUhm2AAe2EqfWM9ucdQzH3xv1HoKoLDqHMK9n";
    auto privateKey = HDWallet::getPrivateKeyFromExtended(xprv, TWCoinType(123456), DerivationPath(TWPurposeBIP44, 123456, 0, 0, 0));
    ASSERT_FALSE(privateKey);
}

TEST(HDWallet, privateKeyFromXPRV_Invalid45) {
    // 45th byte is not 0
    const std::string xprv = "xprv9yqEgpMG2KCjvotCxaiMkzmKJpDXz2xZi3yUe4XsURvo9DUbPySW1qRbhw2dJ8QexahgVSfkjxU4FgmN4GLGN3Ui8oLqC6433CeyPUNVHHh";
    auto privateKey = HDWallet::getPrivateKeyFromExtended(xprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 3));
    ASSERT_FALSE(privateKey);
}

TEST(HDWallet, privateKeyFromMptv) {
    const std::string mptv = "Mtpv7SkyM349Svcf1WiRtB5hC91ZZkVsGuv3kz1V7tThGxBFBzBLFnw6LpaSvwpHHuy8dAfMBqpBvaSAHzbffvhj2TwfojQxM7Ppm3CzW67AFL5";
    auto privateKey = HDWallet::getPrivateKeyFromExtended(mptv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoinCash), 0, 0, 4));
    auto publicKey = privateKey->getPublicKey(TWPublicKeyTypeSECP256k1);

    auto witness = Data{0x00, 0x14};
    auto keyHash = Hash::sha256ripemd(publicKey.bytes.data(), 33);
    witness.insert(witness.end(), keyHash.begin(), keyHash.end());

    auto prefix = Data{TW::p2shPrefix(TWCoinTypeLitecoin)};
    auto redeemScript = Hash::sha256ripemd(witness.data(), witness.size());
    prefix.insert(prefix.end(), redeemScript.begin(), redeemScript.end());

    auto address = Bitcoin::Address(prefix);

    EXPECT_EQ(hex(publicKey.bytes), "02c36f9c3051e9cfbb196ecc35311f3ad705ea6798ffbe6b039e70f6bd047e6f2c");
    EXPECT_EQ(address.string(), "MBzcCaoLk9626cLj2UVvcxs6nsVUi39zEy");
}

TEST(HDWallet, privateKeyFromZprv) {
    const std::string zprv = "zprvAdzGEQ44z4WPLNCRpDaup2RumWxLGgR8PQ9UVsSmJigXsHVDaHK1b6qGM2u9PmxB2Gx264ctAz4yRoN3Xwf1HZmKcn6vmjqwsawF4WqQjfd";
    auto privateKey = HDWallet::getPrivateKeyFromExtended(zprv, TWCoinTypeBitcoinCash, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeBitcoin), 0, 0, 5));
    auto publicKey = privateKey->getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Bitcoin::SegwitAddress(publicKey, 0, "bc");

    EXPECT_EQ(hex(publicKey.bytes), "022dc3f5a3fcfd2d1cc76d0cb386eaad0e30247ba729da0d8847a2713e444fdafa");
    EXPECT_EQ(address.string(), "bc1q5yyq60jepll68hds7exa7kpj20gsvdu0aztw5x");
}

TEST(HDWallet, privateKeyFromDGRV) {
    const std::string dgpv = "dgpv595jAJYGBLanByCJXRzrWBZFVXdNisfuPmKRDquCQcwBbwKbeR21AtkETf4EpjBsfsK3kDZgMqhcuky1B9PrT5nxiEcjghxpUVYviHXuCmc";
    auto privateKey = HDWallet::getPrivateKeyFromExtended(dgpv, TWCoinTypeDogecoin, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeDogecoin), 0, 0, 1));
    auto publicKey = privateKey->getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Bitcoin::Address(publicKey, TW::p2pkhPrefix(TWCoinTypeDogecoin));

    EXPECT_EQ(hex(publicKey.bytes), "03eb6bf281990ee074a39c71ed8ce78c486066ac433bcf066dd5eb08f87d3a6c34");
    EXPECT_EQ(address.string(), "D5taDndQJ1fDF3AM1yWavmJY2BgSi17CUv");
}

TEST(HDWallet, privateKeyFromXPRVForDGB) {
    const std::string xprvForDgb = "xprv9ynLofyuR3uCqCMJADwzBaPnXB53EVe5oLujvPfdvCxae3NzgEpYjZMgcUeS8EUeYfYVLG61ZgPXm9TZWiwBnLVCgd551vCwpXC19hX3mFJ";
    auto privateKey = HDWallet::getPrivateKeyFromExtended(xprvForDgb, TWCoinTypeDigiByte, DerivationPath(TWPurposeBIP44, TWCoinTypeSlip44Id(TWCoinTypeDigiByte), 0, 0, 1));
    auto publicKey = privateKey->getPublicKey(TWPublicKeyTypeSECP256k1);
    auto address = Bitcoin::Address(publicKey, TW::p2pkhPrefix(TWCoinTypeDigiByte));

    EXPECT_EQ(hex(publicKey.bytes), "03238a5c541c2cbbf769dbe0fb2a373c22db4da029370767fbe746d59da4de07f1");
    EXPECT_EQ(address.string(), "D9Gv7jWSVsS9Y5q98C79WyfEj6P2iM5Nzs");
}

TEST(HDWallet, DeriveWithLeadingZerosEth) {
    // Derivation test case with leading zeroes, see  https://blog.polychainlabs.com/bitcoin,/bip32,/bip39,/kdf/2021/05/17/inconsistent-bip32-derivations.html
    const auto mnemonic = "name dash bleak force moral disease shine response menu rescue more will";
    const auto derivationPath = "m/44'/60'";
    const auto coin = TWCoinTypeEthereum;
    auto wallet = HDWallet(mnemonic, "");
    const auto addr = Ethereum::Address(wallet.getKey(coin, DerivationPath(derivationPath)).getPublicKey(TW::publicKeyType(coin)));
    EXPECT_EQ(addr.string(), "0x0ba17e928471c64AaEaf3ABfB3900EF4c27b380D");
}

} // namespace
