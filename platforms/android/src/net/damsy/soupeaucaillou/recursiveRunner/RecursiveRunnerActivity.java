/*
    This file is part of Dogtag.

    @author Soupe au Caillou - Jordane Pelloux-Prayer
    @author Soupe au Caillou - Gautier Pelloux-Prayer
    @author Soupe au Caillou - Pierre-Eric Pelloux-Prayer

    Dogtag is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3.

    Dogtag is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Dogtag.  If not, see <http://www.gnu.org/licenses/>.
*/


package net.damsy.soupeaucaillou.recursiveRunner;

import net.damsy.soupeaucaillou.recursiveRunner.R;
import net.damsy.soupeaucaillou.SacActivity;
import net.damsy.soupeaucaillou.api.AdAPI;
import net.damsy.soupeaucaillou.api.AssetAPI;
import net.damsy.soupeaucaillou.api.CommunicationAPI;
import net.damsy.soupeaucaillou.api.ExitAPI;
import net.damsy.soupeaucaillou.api.LocalizeAPI;
import net.damsy.soupeaucaillou.api.MusicAPI;
import net.damsy.soupeaucaillou.api.SoundAPI;
import net.damsy.soupeaucaillou.api.StorageAPI;
import net.damsy.soupeaucaillou.api.VibrateAPI;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.os.Vibrator;
import android.widget.Toast;

public class RecursiveRunnerActivity extends SacActivity {
	static {
        System.loadLibrary("sac");
    }

    public int getLayoutId() {
        return R.layout.main;
    }

	public int getParentViewId() {
        return R.id.parent_frame;
    }

	@Override
    protected void onCreate(Bundle savedInstanceState) {
		SacActivity.LogI("-> onCreate [" + savedInstanceState);
        super.onCreate(savedInstanceState);

        // Vibrator vibrator = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
        
		try {
			PackageInfo pInfo = getPackageManager().getPackageInfo(getPackageName(), 0);
			Toast toast = Toast.makeText(this, "Package name: " + getPackageName() + ", version code: "
					+ pInfo.versionCode + ", version name: " + pInfo.versionName, Toast.LENGTH_LONG);
			toast.show();
		} catch (NameNotFoundException e) {
			e.printStackTrace();
		}
    }

	@Override
	public void initRequiredAPI() {
		StorageAPI.Instance().init(getApplicationContext());
		VibrateAPI.Instance().init((Vibrator) getSystemService(Context.VIBRATOR_SERVICE));
		AssetAPI.Instance().init(this, getAssets());
		SoundAPI.Instance().init(getAssets());
		LocalizeAPI.Instance().init(this.getResources(), this.getPackageName());
	}
}