all:
	python3 -m grpc_tools.protoc -I. --python_out=. --grpc_python_out=. functions.proto
run_cli_pares:
	make
	python3 client_par.py $(arg)
run_cli_central:
	make
	python3 client_cent.py $(arg)
run_serv_pares_1:
	make
	python3 server_par.py $(arg)
run_serv_pares_2:
	make
	python3 server_par.py $(arg) 1
run_serv_central:
	make
	python3 server_cent.py $(arg)
clean:
	rm functions_pb2_grpc.py functions_pb2.py
