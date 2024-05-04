import gradio_client

def RunTryOnGenerator(a : str, b : str):
    client = gradio_client.Client("https://levihsu-ootdiffusion.hf.space/--replicas/sfxdg/")    
    result = client.predict(
        gradio_client.file(a),  # filepath  in 'Model' Image component
        gradio_client.file(b),   # filepath  in 'Garment' Image component
        # "Upper-body",
        1,      # float (numeric value between 1 and 4) in 'Images' Slider component
        40,     # float (numeric value between 20 and 40) in 'Steps' Slider component
        4,      # float (numeric value between 1.0 and 5.0) in 'Guidance scale' Slider component
        -1,     # float (numeric value between -1 and 2147483647) in 'Seed' Slider component
        api_name="/process_hd"
    )
    print(result)

# RunTryOnGenerator("/home/kirsiv40/qtbsu/visualizer/pic/basemodel2.png", "/home/kirsiv40/qtbsu/visualizer/pic/top222.JPG")
RunTryOnGenerator(input(), input())