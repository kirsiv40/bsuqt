import gradio_client
import sys


# TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
def Gen(a : str, b : str, c : str):
    client = gradio_client.Client("https://levihsu-ootdiffusion.hf.space/")    
    result = client.predict(
        gradio_client.file(a),  # filepath  in 'Model' Image component
        gradio_client.file(b),   # filepath  in 'Garment' Image component
        c,      # "Upper-body", or another
        1,      # float (numeric value between 1 and 4) in 'Images' Slider component
        30,     # float (numeric value between 20 and 40) in 'Steps' Slider component
        3,      # float (numeric value between 1.0 and 5.0) in 'Guidance scale' Slider component
        -1,     # float (numeric value between -1 and 2147483647) in 'Seed' Slider component
        api_name="/process_dc"
    )
    return result[0]['image']

def GenTop(a : str, b : str):
    client = gradio_client.Client("https://levihsu-ootdiffusion.hf.space/")    
    result = client.predict(
        gradio_client.file(a),  # filepath  in 'Model' Image component
        gradio_client.file(b),   # filepath  in 'Garment' Image component
        1,      # float (numeric value between 1 and 4) in 'Images' Slider component
        30,     # float (numeric value between 20 and 40) in 'Steps' Slider component
        3,      # float (numeric value between 1.0 and 5.0) in 'Guidance scale' Slider component
        -1,     # float (numeric value between -1 and 2147483647) in 'Seed' Slider component
        api_name="/process_hd"
    )
    return result[0]['image']


def RunTryOnGenerator():
    with open(sys.argv[3], "w") as f:
        try:
            if (sys.argv[4] == "Upper-body"):
                res = GenTop(sys.argv[1], sys.argv[2])
            else:
                res = Gen(sys.argv[1], sys.argv[2], sys.argv[4])
            f.write(res)
            # sys.argv[2] = 10
        except(ValueError):
            print("Bad input files")


RunTryOnGenerator()