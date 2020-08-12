struct ps_in
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
};
Texture2D myTexture: register(t0);
SamplerState samplerState;


float4 ps_main(ps_in input) : SV_TARGET
{
	float4 tempColor = 0;
	float3 blurColor = 0;
	bool brightPixelFound = false;

	//brightness 1 = max of one color
	float brightness = 1;

	//pixelrange 
	int range = 6;

	//Unroll could work instead
	for (int x = -range; x <= range; x++)
	{
		for (int y = -range; y <= range; y++)
		{
			//Third argument is offset
			tempColor = myTexture.Sample(samplerState, input.texcoord, int2(x, y));
			
			//length(tempColor.rgb) becomes vector for checking of brightness

			//Writes .rgb to make sure we don't account for the alpha channel
			if (length(tempColor.rgb) >= brightness && tempColor.a != 0.f)
			{
				brightPixelFound = true;
			}

			//Writes .rgb to make sure we don't account for the alpha channel when adding the values
			blurColor += tempColor.rgb;
		}
	}

	//Getting an average value depending on all surrounding pixels within range in both x and y
	blurColor /= ((range * 2 + 1) * (range * 2 + 1));
	
	if (brightPixelFound == true)
	{
		return float4(blurColor, 1.f);
	}

	return myTexture.Sample(samplerState, input.texcoord);
}