import numpy as np
import argparse
from lempel_ziv import lz78_decode,lz78_encode


if __name__ == "__main__":
    # Recebe os arguemntos da linha de comando
    parser = argparse.ArgumentParser()
    parser.add_argument("-c",type=str,help="Indica o nome do arquivo para compressão")
    parser.add_argument("-d",type=str,help="Indica o nome do arquivo para descompressão")
    parser.add_argument("-o",type=str,help="Indica o nome do arquivo que será salvo")

    args = parser.parse_args()

    # Caso se queira comprimir
    if args.c != None:

        outfile_suffix = ".z78"
        
        # Carregamos os bits do arquivo
        loaded_bytes = np.fromfile(args.c, dtype = "uint8")
        bit_array = np.unpackbits(loaded_bytes)

        bstr = "".join([str(i) for i in bit_array])

        # Realizamos o encoding dos bits
        encoded_str = lz78_encode(bstr)

        # Decidimos o nome do arquivo que será salvo
        if args.o != None:
            outfile_name = args.o + outfile_suffix
        else:
            outfile_name = args.c.split(".")[0] + outfile_suffix

        # Convertemos a string de bits para bytes e então salvamos o arquivo 
        #(Note que, como salvaremos os bytes no arquivo txt, ao abrir o respectivo arquivo em um leitor de texto vizualiaremos apenas os simbolos que correspondem aos bytes, 
        #e não os bytes em formato binário)
        np.packbits(np.array([int(i,2) for i in encoded_str])).tofile(outfile_name)

    elif args.d != None:
        outfile_suffix = ".txt"

        loaded_bytes = np.fromfile(args.d, dtype = "uint8")
        bit_array = np.unpackbits(loaded_bytes)

        encoded_str = "".join([str(i) for i in bit_array])

        decoded_str = lz78_decode(encoded_str)

        if args.o != None:
            outfile_name = args.o + outfile_suffix
        else:
            outfile_name = args.d.split(".")[0] + "_dec"+ outfile_suffix

        np.packbits(np.array([int(i,2) for i in decoded_str])).tofile(outfile_name)
