package com.trustwallet.core.app.blockchains.ethereum

import com.trustwallet.core.app.utils.Numeric
import org.junit.Assert.assertEquals
import org.junit.Test
import org.junit.Assert.assertFalse
import wallet.core.jni.proto.Barz
import wallet.core.jni.LiquidStaking as WCBarz

class TestBarz {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testCounterfactualAddressFromPublicKey() {
        val input = WCBarz.ContractAddressInput.newBuilder()
        input.apply {
            factory = "0xb7f8bc63bbcad18155201308c8f3540b07f84f5e"
            diamondCutFacet = "0xcf7ed3acca5a467e9e704c703e8d87f634fb0fc9"
            accountFacet = "0xdc64a140aa3e981100a9beca4e685f962f0cf6c9"
            verificationFacet = "0x5fc8d32690cc91d4c39d9d3abcbd16989f875707"
            entryPoint = "0xf39fd6e51aad88f6f4ce6ab8827279cfffb92266"
            securityManager = "0xe7f1725e7734ce288f8367e1bb143e90bb3f0512"
            facetRegistry = "0x9fe46736679d2d9a65f0992f2272de9f3c7fa6e0"
            bytecode = "0x60808060405261127d80380380916100178285610943565b833981019060e0818303126108de5761002f81610966565b9161003c60208301610966565b9161004960408201610966565b9161005660608301610966565b9161006360808201610966565b61006f60a08301610966565b60c083015190926001600160401b0382116108de570183601f820112156108de57805161009b8161097a565b946100a96040519687610943565b818652602082840101116108de576100c79160208087019101610995565b604051633253960f60e01b815260208160048160006001600160a01b038b165af19081156108eb576000916108a3575b506001600160e01b031981161561085f576040979695975196610119886108f7565b6002885260005b6040811061082b57509161026693916102749695936040519061014282610928565b60018252602036818401376040519161015a83610928565b60018352602036818501376307e4c70760e21b610176826109b8565b5260405191610184836108f7565b6001600160a01b031682526000602083015260408201526101a48b6109b8565b526101ae8a6109b8565b506001600160e01b031982166101c3826109b8565b52604051906101d1826108f7565b6001600160a01b038c1682526000602083015260408201526101f28a6109db565b526101fc896109db565b506001805464ffffffff00191660d89290921c64ffffffff0016919091179055604051634a93641760e01b60208201526001600160a01b03958616602482015298851660448a015284166064890152909216608487015260a060a4870152859160c48301906109ff565b03601f198101855284610943565b6000915b805183101561070457602061028d84836109eb565b51015160038110156106ee578061041457506001600160a01b036102b184836109eb565b5151169160406102c185846109eb565b510151916102d183511515610a49565b6102dc841515610aa9565b6001600160a01b038416600090815260008051602061125d83398151915260205260409020546001600160601b0316918215610406575b6000925b84518410156103ec576001600160e01b031961033385876109eb565b5116600081815260008051602061121d83398151915260205260409020546001600160a01b031661038157816103708861037b9461037594610c3b565b610b0a565b93610a24565b92610317565b60405162461bcd60e51b815260206004820152603560248201527f4c69624469616d6f6e644375743a2043616e2774206164642066756e6374696f60448201527f6e207468617420616c72656164792065786973747300000000000000000000006064820152608490fd5b509491509492506103fe91505b610a24565b919092610278565b61040f85610b49565b610313565b9193916001810361058d57506001600160a01b0361043284836109eb565b51511693604061044285846109eb565b5101519161045283511515610a49565b61045d861515610aa9565b6001600160a01b038616600090815260008051602061125d83398151915260205260409020546001600160601b031691821561057f575b6000925b845184101561056f576001600160e01b03196104b485876109eb565b5116600081815260008051602061121d83398151915260205260409020546001600160a01b031691898314610504576103708a82846104f9610375966104fe98610d26565b610c3b565b92610498565b60405162461bcd60e51b815260206004820152603860248201527f4c69624469616d6f6e644375743a2043616e2774207265706c6163652066756e60448201527f6374696f6e20776974682073616d652066756e6374696f6e00000000000000006064820152608490fd5b5094915094506103fe9150610a24565b61058887610b49565b610494565b600203610699576001600160a01b036105a684836109eb565b5151169360406105b685846109eb565b510151946105c686511515610a49565b61062e5760005b85518110156106215761061c906103f96001600160e01b03196105f0838a6109eb565b5116600081815260008051602061121d83398151915260205260409020546001600160a01b0316610d26565b6105cd565b50926103fe919450610a24565b60405162461bcd60e51b815260206004820152603660248201527f4c69624469616d6f6e644375743a2052656d6f7665206661636574206164647260448201527f657373206d7573742062652061646472657373283029000000000000000000006064820152608490fd5b60405162461bcd60e51b815260206004820152602760248201527f4c69624469616d6f6e644375743a20496e636f727265637420466163657443756044820152663a20b1ba34b7b760c91b6064820152608490fd5b634e487b7160e01b600052602160045260246000fd5b8382604051606081016060825284518091526080820190602060808260051b8501019601916000905b828210610793576001600160a01b0386166020860152848803604086015261078587877f8faa70878671ccd212d20771b795c50af8fd3ff6cf27f4bde57e5d4de0aeb673888061077d8e866109ff565b0390a161101f565b60405160e2908161113b8239f35b848803607f19018152835180516001600160a01b03168952602081015194989394929391929060038210156106ee576040916020840152015190606060408201526020608060608301928451809452019201906000905b8082106108085750505060208060019299019201920190929161072d565b82516001600160e01b0319168452602093840193909201916001909101906107ea565b60209060409a98999a5161083e816108f7565b600081526000838201526060604082015282828d0101520198979698610120565b606460405162461bcd60e51b815260206004820152602060248201527f4261727a3a20496e76616c696420566572696669636174696f6e2046616365746044820152fd5b90506020813d6020116108e3575b816108be60209383610943565b810103126108de57516001600160e01b0319811681036108de57386100f7565b600080fd5b3d91506108b1565b6040513d6000823e3d90fd5b606081019081106001600160401b0382111761091257604052565b634e487b7160e01b600052604160045260246000fd5b604081019081106001600160401b0382111761091257604052565b601f909101601f19168101906001600160401b0382119082101761091257604052565b51906001600160a01b03821682036108de57565b6001600160401b03811161091257601f01601f191660200190565b60005b8381106109a85750506000910152565b8181015183820152602001610998565b8051156109c55760200190565b634e487b7160e01b600052603260045260246000fd5b8051600110156109c55760400190565b80518210156109c55760209160051b010190565b90602091610a1881518092818552858086019101610995565b601f01601f1916010190565b6000198114610a335760010190565b634e487b7160e01b600052601160045260246000fd5b15610a5057565b60405162461bcd60e51b815260206004820152602b60248201527f4c69624469616d6f6e644375743a204e6f2073656c6563746f727320696e206660448201526a1858d95d081d1bc818dd5d60aa1b6064820152608490fd5b15610ab057565b60405162461bcd60e51b815260206004820152602c60248201527f4c69624469616d6f6e644375743a204164642066616365742063616e2774206260448201526b65206164647265737328302960a01b6064820152608490fd5b6001600160601b03908116908114610a335760010190565b60008051602061123d83398151915280548210156109c55760005260206000200190600090565b610b95604051610b58816108f7565b602481527f4c69624469616d6f6e644375743a204e657720666163657420686173206e6f20602082015263636f646560e01b60408201528261110d565b60008051602061123d83398151915280546001600160a01b038316600090815260008051602061125d833981519152602052604090206001018190559190680100000000000000008310156109125782610bf7916001610c1695019055610b22565b90919082549060031b9160018060a01b03809116831b921b1916179055565b565b91909180548310156109c557600052601c60206000208360031c019260021b1690565b6001600160e01b03198116600081815260008051602061121d83398151915260208190526040822080546001600160a01b031660a09690961b6001600160a01b031916959095179094559194939092906001600160a01b031680835260008051602061125d8339815191526020526040832080549194919068010000000000000000821015610d125796610cdc8260409798996001610cf995018155610c18565b90919063ffffffff83549160031b9260e01c831b921b1916179055565b82526020522080546001600160a01b0319169091179055565b634e487b7160e01b85526041600452602485fd5b9091906001600160a01b039081168015610fb457308114610f585763ffffffff60e01b80941660009281845260008051602061121d833981519152926020918483526040948587205460a01c9083885260008051602061125d8339815191529586865287892054926000199b8c8501948511610f4457908991888c898c89808703610ed6575b505090525050508787525087892080548015610ec2578c0190610dcf8282610c18565b63ffffffff82549160031b1b191690555588528452868681205515610df9575b5050505050509050565b60008051602061123d8339815191528054898101908111610eae57838852858552826001888a20015491808303610e7c575b5050508054988915610e685760019798990191610e4783610b22565b909182549160031b1b19169055558552528220015580388080808080610def565b634e487b7160e01b88526031600452602488fd5b610e8590610b22565b90549060031b1c16610e9a81610bf784610b22565b885285855260018789200155388281610e2b565b634e487b7160e01b88526011600452602488fd5b634e487b7160e01b8b52603160045260248bfd5b610f379784610cdc93610ef58a9487610f0b9952828a52848420610c18565b90549060031b1c60e01b97889683525220610c18565b168b52838852898b2080546001600160a01b031660a09290921b6001600160a01b031916919091179055565b873880888c898c89610dac565b634e487b7160e01b8b52601160045260248bfd5b60405162461bcd60e51b815260206004820152602e60248201527f4c69624469616d6f6e644375743a2043616e27742072656d6f766520696d6d7560448201526d3a30b1363290333ab731ba34b7b760911b6064820152608490fd5b60405162461bcd60e51b815260206004820152603760248201527f4c69624469616d6f6e644375743a2043616e27742072656d6f76652066756e6360448201527f74696f6e207468617420646f65736e27742065786973740000000000000000006064820152608490fd5b6001600160a01b038116919082156111085760008091611085604051611044816108f7565b602881527f4c69624469616d6f6e644375743a205f696e6974206164647265737320686173602082015267206e6f20636f646560c01b60408201528261110d565b83519060208501905af4913d15611100573d926110a18461097a565b936110af6040519586610943565b84523d6000602086013e5b156110c457505050565b8251156110d357825160208401fd5b6110fc60405192839263192105d760e01b845260048401526040602484015260448301906109ff565b0390fd5b6060926110ba565b505050565b3b156111165750565b60405162461bcd60e51b8152602060048201529081906110fc9060248301906109ff56fe6080604052361560aa57600080356001600160e01b03191681527f183cde5d4f6bb7b445b8fc2f7f15d0fd1d162275aded24183babbffee7cd491f60205260408120546001600160a01b03168015606c57818091368280378136915af43d82803e156068573d90f35b3d90fd5b62461bcd60e51b6080526020608452602060a4527f4469616d6f6e643a2046756e6374696f6e20646f6573206e6f7420657869737460c45260646080fd5b00fea26469706673582212203df3feea7f1aa6ea31eb82d2ad84790e47617c094ced4ce17015a8a7487295da64736f6c63430008120033183cde5d4f6bb7b445b8fc2f7f15d0fd1d162275aded24183babbffee7cd491f183cde5d4f6bb7b445b8fc2f7f15d0fd1d162275aded24183babbffee7cd4921183cde5d4f6bb7b445b8fc2f7f15d0fd1d162275aded24183babbffee7cd4920"
            owner = WCBarz.ContractOwner.newBuilder().apply {
                publicKey = "0x019b4ee7ad22ffd4c215e5f424faf4c75577dc36"
            }.build()
        }.build()
        val result = WCBarz.getCounterfactualAddress(input)
        assertEquals(result, "0xE9C377EFCE644fd54614d77C177c14148502496F")
    }

    @Test
    fun testCounterfactualAddressFromAttestationObject() {
        val input = WCBarz.ContractAddressInput.newBuilder()
        input.apply {
            factory = "0xb7f8bc63bbcad18155201308c8f3540b07f84f5e"
            diamondCutFacet = "0xcf7ed3acca5a467e9e704c703e8d87f634fb0fc9"
            accountFacet = "0xdc64a140aa3e981100a9beca4e685f962f0cf6c9"
            verificationFacet = "0x5fc8d32690cc91d4c39d9d3abcbd16989f875707"
            entryPoint = "0xf39fd6e51aad88f6f4ce6ab8827279cfffb92266"
            securityManager = "0xe7f1725e7734ce288f8367e1bb143e90bb3f0512"
            facetRegistry = "0x9fe46736679d2d9a65f0992f2272de9f3c7fa6e0"
            bytecode = "0x60808060405261127d80380380916100178285610943565b833981019060e0818303126108de5761002f81610966565b9161003c60208301610966565b9161004960408201610966565b9161005660608301610966565b9161006360808201610966565b61006f60a08301610966565b60c083015190926001600160401b0382116108de570183601f820112156108de57805161009b8161097a565b946100a96040519687610943565b818652602082840101116108de576100c79160208087019101610995565b604051633253960f60e01b815260208160048160006001600160a01b038b165af19081156108eb576000916108a3575b506001600160e01b031981161561085f576040979695975196610119886108f7565b6002885260005b6040811061082b57509161026693916102749695936040519061014282610928565b60018252602036818401376040519161015a83610928565b60018352602036818501376307e4c70760e21b610176826109b8565b5260405191610184836108f7565b6001600160a01b031682526000602083015260408201526101a48b6109b8565b526101ae8a6109b8565b506001600160e01b031982166101c3826109b8565b52604051906101d1826108f7565b6001600160a01b038c1682526000602083015260408201526101f28a6109db565b526101fc896109db565b506001805464ffffffff00191660d89290921c64ffffffff0016919091179055604051634a93641760e01b60208201526001600160a01b03958616602482015298851660448a015284166064890152909216608487015260a060a4870152859160c48301906109ff565b03601f198101855284610943565b6000915b805183101561070457602061028d84836109eb565b51015160038110156106ee578061041457506001600160a01b036102b184836109eb565b5151169160406102c185846109eb565b510151916102d183511515610a49565b6102dc841515610aa9565b6001600160a01b038416600090815260008051602061125d83398151915260205260409020546001600160601b0316918215610406575b6000925b84518410156103ec576001600160e01b031961033385876109eb565b5116600081815260008051602061121d83398151915260205260409020546001600160a01b031661038157816103708861037b9461037594610c3b565b610b0a565b93610a24565b92610317565b60405162461bcd60e51b815260206004820152603560248201527f4c69624469616d6f6e644375743a2043616e2774206164642066756e6374696f60448201527f6e207468617420616c72656164792065786973747300000000000000000000006064820152608490fd5b509491509492506103fe91505b610a24565b919092610278565b61040f85610b49565b610313565b9193916001810361058d57506001600160a01b0361043284836109eb565b51511693604061044285846109eb565b5101519161045283511515610a49565b61045d861515610aa9565b6001600160a01b038616600090815260008051602061125d83398151915260205260409020546001600160601b031691821561057f575b6000925b845184101561056f576001600160e01b03196104b485876109eb565b5116600081815260008051602061121d83398151915260205260409020546001600160a01b031691898314610504576103708a82846104f9610375966104fe98610d26565b610c3b565b92610498565b60405162461bcd60e51b815260206004820152603860248201527f4c69624469616d6f6e644375743a2043616e2774207265706c6163652066756e60448201527f6374696f6e20776974682073616d652066756e6374696f6e00000000000000006064820152608490fd5b5094915094506103fe9150610a24565b61058887610b49565b610494565b600203610699576001600160a01b036105a684836109eb565b5151169360406105b685846109eb565b510151946105c686511515610a49565b61062e5760005b85518110156106215761061c906103f96001600160e01b03196105f0838a6109eb565b5116600081815260008051602061121d83398151915260205260409020546001600160a01b0316610d26565b6105cd565b50926103fe919450610a24565b60405162461bcd60e51b815260206004820152603660248201527f4c69624469616d6f6e644375743a2052656d6f7665206661636574206164647260448201527f657373206d7573742062652061646472657373283029000000000000000000006064820152608490fd5b60405162461bcd60e51b815260206004820152602760248201527f4c69624469616d6f6e644375743a20496e636f727265637420466163657443756044820152663a20b1ba34b7b760c91b6064820152608490fd5b634e487b7160e01b600052602160045260246000fd5b8382604051606081016060825284518091526080820190602060808260051b8501019601916000905b828210610793576001600160a01b0386166020860152848803604086015261078587877f8faa70878671ccd212d20771b795c50af8fd3ff6cf27f4bde57e5d4de0aeb673888061077d8e866109ff565b0390a161101f565b60405160e2908161113b8239f35b848803607f19018152835180516001600160a01b03168952602081015194989394929391929060038210156106ee576040916020840152015190606060408201526020608060608301928451809452019201906000905b8082106108085750505060208060019299019201920190929161072d565b82516001600160e01b0319168452602093840193909201916001909101906107ea565b60209060409a98999a5161083e816108f7565b600081526000838201526060604082015282828d0101520198979698610120565b606460405162461bcd60e51b815260206004820152602060248201527f4261727a3a20496e76616c696420566572696669636174696f6e2046616365746044820152fd5b90506020813d6020116108e3575b816108be60209383610943565b810103126108de57516001600160e01b0319811681036108de57386100f7565b600080fd5b3d91506108b1565b6040513d6000823e3d90fd5b606081019081106001600160401b0382111761091257604052565b634e487b7160e01b600052604160045260246000fd5b604081019081106001600160401b0382111761091257604052565b601f909101601f19168101906001600160401b0382119082101761091257604052565b51906001600160a01b03821682036108de57565b6001600160401b03811161091257601f01601f191660200190565b60005b8381106109a85750506000910152565b8181015183820152602001610998565b8051156109c55760200190565b634e487b7160e01b600052603260045260246000fd5b8051600110156109c55760400190565b80518210156109c55760209160051b010190565b90602091610a1881518092818552858086019101610995565b601f01601f1916010190565b6000198114610a335760010190565b634e487b7160e01b600052601160045260246000fd5b15610a5057565b60405162461bcd60e51b815260206004820152602b60248201527f4c69624469616d6f6e644375743a204e6f2073656c6563746f727320696e206660448201526a1858d95d081d1bc818dd5d60aa1b6064820152608490fd5b15610ab057565b60405162461bcd60e51b815260206004820152602c60248201527f4c69624469616d6f6e644375743a204164642066616365742063616e2774206260448201526b65206164647265737328302960a01b6064820152608490fd5b6001600160601b03908116908114610a335760010190565b60008051602061123d83398151915280548210156109c55760005260206000200190600090565b610b95604051610b58816108f7565b602481527f4c69624469616d6f6e644375743a204e657720666163657420686173206e6f20602082015263636f646560e01b60408201528261110d565b60008051602061123d83398151915280546001600160a01b038316600090815260008051602061125d833981519152602052604090206001018190559190680100000000000000008310156109125782610bf7916001610c1695019055610b22565b90919082549060031b9160018060a01b03809116831b921b1916179055565b565b91909180548310156109c557600052601c60206000208360031c019260021b1690565b6001600160e01b03198116600081815260008051602061121d83398151915260208190526040822080546001600160a01b031660a09690961b6001600160a01b031916959095179094559194939092906001600160a01b031680835260008051602061125d8339815191526020526040832080549194919068010000000000000000821015610d125796610cdc8260409798996001610cf995018155610c18565b90919063ffffffff83549160031b9260e01c831b921b1916179055565b82526020522080546001600160a01b0319169091179055565b634e487b7160e01b85526041600452602485fd5b9091906001600160a01b039081168015610fb457308114610f585763ffffffff60e01b80941660009281845260008051602061121d833981519152926020918483526040948587205460a01c9083885260008051602061125d8339815191529586865287892054926000199b8c8501948511610f4457908991888c898c89808703610ed6575b505090525050508787525087892080548015610ec2578c0190610dcf8282610c18565b63ffffffff82549160031b1b191690555588528452868681205515610df9575b5050505050509050565b60008051602061123d8339815191528054898101908111610eae57838852858552826001888a20015491808303610e7c575b5050508054988915610e685760019798990191610e4783610b22565b909182549160031b1b19169055558552528220015580388080808080610def565b634e487b7160e01b88526031600452602488fd5b610e8590610b22565b90549060031b1c16610e9a81610bf784610b22565b885285855260018789200155388281610e2b565b634e487b7160e01b88526011600452602488fd5b634e487b7160e01b8b52603160045260248bfd5b610f379784610cdc93610ef58a9487610f0b9952828a52848420610c18565b90549060031b1c60e01b97889683525220610c18565b168b52838852898b2080546001600160a01b031660a09290921b6001600160a01b031916919091179055565b873880888c898c89610dac565b634e487b7160e01b8b52601160045260248bfd5b60405162461bcd60e51b815260206004820152602e60248201527f4c69624469616d6f6e644375743a2043616e27742072656d6f766520696d6d7560448201526d3a30b1363290333ab731ba34b7b760911b6064820152608490fd5b60405162461bcd60e51b815260206004820152603760248201527f4c69624469616d6f6e644375743a2043616e27742072656d6f76652066756e6360448201527f74696f6e207468617420646f65736e27742065786973740000000000000000006064820152608490fd5b6001600160a01b038116919082156111085760008091611085604051611044816108f7565b602881527f4c69624469616d6f6e644375743a205f696e6974206164647265737320686173602082015267206e6f20636f646560c01b60408201528261110d565b83519060208501905af4913d15611100573d926110a18461097a565b936110af6040519586610943565b84523d6000602086013e5b156110c457505050565b8251156110d357825160208401fd5b6110fc60405192839263192105d760e01b845260048401526040602484015260448301906109ff565b0390fd5b6060926110ba565b505050565b3b156111165750565b60405162461bcd60e51b8152602060048201529081906110fc9060248301906109ff56fe6080604052361560aa57600080356001600160e01b03191681527f183cde5d4f6bb7b445b8fc2f7f15d0fd1d162275aded24183babbffee7cd491f60205260408120546001600160a01b03168015606c57818091368280378136915af43d82803e156068573d90f35b3d90fd5b62461bcd60e51b6080526020608452602060a4527f4469616d6f6e643a2046756e6374696f6e20646f6573206e6f7420657869737460c45260646080fd5b00fea26469706673582212203df3feea7f1aa6ea31eb82d2ad84790e47617c094ced4ce17015a8a7487295da64736f6c63430008120033183cde5d4f6bb7b445b8fc2f7f15d0fd1d162275aded24183babbffee7cd491f183cde5d4f6bb7b445b8fc2f7f15d0fd1d162275aded24183babbffee7cd4921183cde5d4f6bb7b445b8fc2f7f15d0fd1d162275aded24183babbffee7cd4920"
            owner = WCBarz.ContractOwner.newBuilder().apply {
                attestationObject = "0xa363666d74646e6f6e656761747453746d74a068617574684461746158a4f95bc73828ee210f9fd3bbe72d97908013b0a3759e9aea3d0ae318766cd2e1ad4500000000adce000235bcc60a648b0b25f1f055030020c720eb493e167ce93183dd91f5661e1004ed8cc1be23d3340d92381da5c0c80ca5010203262001215820a620a8cfc88fd062b11eab31663e56cad95278bef612959be214d98779f645b82258204e7b905b42917570148b0432f99ba21f2e7eebe018cbf837247e38150a89f771"
            }.build()
        }.build()
        val result = WCBarz.getCounterfactualAddress(input)
        assertEquals(result, "0xc3d477e5D4CEd32835574bBAFE51a0A8c9c17272")
    }
}
