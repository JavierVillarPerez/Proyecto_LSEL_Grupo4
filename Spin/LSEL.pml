#define s_sleep 0
#define s_setup 1
#define s_measure 2
#define s_process 3
#define s_warning 4

ltl p_error1 {
	[] (((state == s_process) && [](range)) -> <> [](alert == 1)) 
}
ltl p_error2 { 
	[] (((state == s_measure) && [](error)) -> <> [](alert == 1))
}

ltl p_error3{
        [] (((state == s_process) && [](!range)) -> <> (state == s_sleep))
}

#define warning_limit 2

int state;
bit timer;
bit error;
bit alert;
bit presence;
bit range;
int warning_counter;


active proctype sensor_fsm (){
	state = s_sleep;
	do
	:: (state == s_sleep) -> atomic{
		printf("Estado : Sleep \n");
		if
		   	:: presence -> presence = 0; state = s_setup;
		   	:: (!presence && timer) -> timer = 0; state = s_setup;
		fi
	}
	:: (state == s_setup) -> atomic{
		printf("Estado : Setup \n");
		if
		   	:: timer -> timer = 0; state =s_measure;
		fi	
	}
	:: (state == s_measure) -> atomic{
		printf("Estado : Measure \n");
		if
			::(error) -> error = 0; state = s_warning;
			::(!error && timer) -> timer = 0; state = s_process;
		fi
	}
	:: (state == s_process) -> atomic{
		printf("Estado : Process \n");
		if
			::(range) ->  range = 0; state = s_warning;
			::(!range) -> state = s_sleep;
		fi
	}
	:: (state == s_warning) -> atomic{
		printf("Estado : Warning \n");  warning_counter ++; alert = 1;
		if
			:: (warning_counter  >= warning_limit && !presence) -> warning_counter = 0; alert = 0; state = s_sleep;
			:: (warning_counter << warning_limit) -> state = s_measure;
		fi
	}
	od
}

active proctype entorno (){
	if
	   	:: presence = 1;
	   	:: error = 1;
	 	:: range = 1;
		:: timer = 1;
	   	:: (!presence && !error && !range && !timer) -> skip
	fi
	printf("Presencia = %d, error = %d, alerta = %d \n", presence, error, alert);
}

