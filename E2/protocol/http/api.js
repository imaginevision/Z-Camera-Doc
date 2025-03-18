class HttpError extends Error {
    constructor(status, message) {
        super(message);
        this.status = status;
        this.name = 'HttpError';
    }
}

class Api {
    static async request(url, options = {}) {
        const timeout = (options && options.timeout) || 20000;
        const fetchOptions = options ? { ...options } : {};
        if (fetchOptions.timeout) {
            delete fetchOptions.timeout;
        }

        const controller = new AbortController();
        const timeoutId = setTimeout(() => controller.abort(), timeout);
        try {
            const response = await fetch(url, {
                ...fetchOptions,
                signal: controller.signal
            });

            if (!response.ok) {
                switch (response.status) {
                    case 401:
                        console.error(`Unauthorized ${url}`);
                        break;
                    case 403:
                        console.error(`Forbidden ${url}`);
                        break;
                    case 404:
                        console.error(`Not Found ${url}`);
                        break;
                    default:
                        console.error(`HTTP error! status: ${response.status} ${url}`);
                        break;
                }
                throw new HttpError(response.status, response.statusText);
            }
            return response.json(); 
        } catch (error) {
            console.error('Fetch error:', error);
            if (error.name === 'AbortError') {
                const timeoutError = new Error(`Request timeout after ${timeout}ms`);
                timeoutError.textStatus = 'timeout';
                throw timeoutError;
            }
            throw error;
        } finally {
            clearTimeout(timeoutId);
        }
    }

    static camera = {
        info: ()=> this.request("/info"),
        mode: () => this.request("/ctrl/mode"),

        nickName: () => this.request("/ctrl/nick_name"),
        setNickName: (name) => this.request(`/ctrl/nick_name?action=set&name=${encodeURIComponent(name)}`),
        commit: () => this.request("/commit_info"),
        gotoRec: () => this.request("/ctrl/mode?action=to_rec"),
        getStatus: ()=> this.request("/camera_status")
    }

    static session = {
        ping: () => this.request("/ctrl/session"),
        quit: () => this.request("/ctrl/session?action=quit")
        /*
        quit: () => {
            if (navigator.sendBeacon) {
                navigator.sendBeacon("/ctrl/session?action=quit");
            } else {
                this.request("/ctrl/session?action=quit");
            }
        }*/
    }

    static ctrl = {
        set: (key, value)=> this.request(`/ctrl/set?${key}=${encodeURIComponent(value)}`),
        get: (key)=> this.request(`/ctrl/get?k=${key}`),
        getCatalog: (catalog)=> this.request(`/ctrl/getbatch?catalog=${catalog}`)
    }

    static lens = {
        focalLength: () => this.ctrl.get("lens_focal_length"),
        zoom : {
            in: (speed) => this.request(`/ctrl/lens?action=zoomin&fspeed=${speed}`),
            out: (speed) => this.request(`/ctrl/lens?action=zoomout&fspeed=${speed}`),
            stop: () => this.request("/ctrl/lens?action=zoomstop"),
            status: () => this.request("/ctrl/lens?action=z_status"),

            zoomMode: () => this.ctrl.get("zoom_mode"),
            setZoomMode: (mode) => this.ctrl.set("zoom_mode", mode)
        },

        focus : {
            near: (speed) => this.request(`/ctrl/lens?action=focusnear&fspeed=${speed}`),
            far: (speed) => this.request(`/ctrl/lens?action=focusfar&fspeed=${speed}`),
            stop: () => this.request("/ctrl/lens?action=focusstop"),
            status: () => this.request("/ctrl/lens?action=f_status")
        },

        af : {
            onePush: () => this.request("/ctrl/af"),

            afSpeed: () => this.ctrl.get("af_speed"),
            setAfSpeed: (speed) => this.ctrl.set("af_speed", speed),

            // AF, MF
            focusMethod: () => this.ctrl.get("focus"),
            setFocusMethod: (method) => this.ctrl.set("focus", method),

            // ROI type
            roiType: () => this.ctrl.get("af_mode"),
            setRoiType: (type) => this.ctrl.set("af_mode", type),
            updateRoiCenter: (x, y) => this.request(`/ctrl/af?action=update_roi_center&x=${x}&y=${y}`),
            getRoi: () => this.request("/ctrl/af?action=query"),

            //Continuous AF
            caf: () => this.ctrl.get("caf"),
            setCaf: (enable) => this.ctrl.set("caf", enable),

            // Flexiable size
            flexiableSize: () => this.ctrl.get("af_area"),
            setFlexiableSize: (size) => this.ctrl.set("af_area", size),

            cafSensitivity: () => this.ctrl.get("caf_sens"),
            setCafSensitivity: (sens) => this.ctrl.set("caf_sens", sens),

            // AF adjust with PTZ
            afAdjustWithPtz: () => this.ctrl.get("af_adjust_with_ptz"),
            setAfAdjustWithPtz: (enable) => this.ctrl.set("af_adjust_with_ptz", enable),

            traceFaceId: (id) => this.request(`/ctrl/af_face/trace_target?id=${id}`),

            //Live CAF
            liveCAF: () => this.ctrl.get("live_caf"),
            setLiveCAF: (enable) => this.ctrl.set("live_caf", enable),

            //MF Assist Preview
            mfAssistPre: () => this.ctrl.get("mf_mag"),
            setMFAssistPre: (enable) => this.ctrl.set("mf_mag", enable),

            //MF Assist Recording
            mfAssistRec: () => this.ctrl.get("mf_recording"),
            setMFAssistRec: (enable) => this.ctrl.set("mf_recording", enable),
        }
    }

    static pt = {
        query: () => this.request("/ctrl/pt?action=query"),
        queryDetail: () => this.request("/ctrl/pt?action=query&detail=y"),

        stop: () => this.request("/ctrl/pt?action=stop"),
        stopAll: () => this.request("/ctrl/pt?action=stop_all"),

        directionMove: (direction, speed) => this.request(`/ctrl/pt?action=${direction}&fspeed=${speed}`),
        ptMove: (pan, tilt) => this.request(`/ctrl/pt?action=pt&pan_speed=${pan}&tilt_speed=${tilt}`),

        home: () => this.request("/ctrl/pt?action=home"),
        reset: () => this.request("/ctrl/pt?action=reset"),
        limitUpdate: (direct, pan_pos, tilt_pos) => this.request(`/ctrl/pt?action=limit&direct=${direct}&pan_pos=${pan_pos}&tilt_pos=${tilt_pos}`),

        setLimit: (enable) => this.ctrl.set("ptz_limit", enable),
        limit: () => this.ctrl.get("ptz_limit"),

        speedMode: () => this.ctrl.get("pt_speedmode"),
        setSpeedMode: (mode) => this.ctrl.set("pt_speedmode", mode),

        speedWithZoomPos: () => this.ctrl.get("pt_speed_with_zoom_pos"),
        setSpeedWithZoomPos: (enable) => this.ctrl.set("pt_speed_with_zoom_pos", enable),

        flip: () => this.ctrl.get("ptz_flip"),
        setFlip: (enable) => this.ctrl.set("ptz_flip", enable),

        privacyMode: () => this.ctrl.get("pt_priv_mode"),
        setPrivacyMode: (enable) => this.ctrl.set("pt_priv_mode", enable),

        powerOnPosition: () => this.ctrl.get("pt_pwr_pos"),
        setPowerOnPosition: (pos) => this.ctrl.set("pt_pwr_pos", pos)
    }

    static preset = {
        recall: (index) => this.request(`/ctrl/preset?action=recall&index=${index}`),
        save: (index) => this.request(`/ctrl/preset?action=set&index=${index}`),
        delete: (index) => this.request(`/ctrl/preset?action=del&index=${index}`),

        getInfo: (index) => this.request(`/ctrl/preset?action=get_info&index=${index}`),
        setName: (index, name) => this.request(`/ctrl/preset?action=set_name&index=${index}&new_name=${encodeURIComponent(name)}`),
        
        setSpeedUnit: (index, unit) => this.request(`/ctrl/preset?action=preset_speed&index=${index}&preset_speed_unit=${unit}`),
        setSpeedByDuraton: (index, time) => this.request(`/ctrl/preset?action=preset_speed&index=${index}&preset_time=${time}`),
        setSpeedByIdex: (index, speed) => this.request(`/ctrl/preset?action=preset_speed&index=${index}&preset_speed=${speed}`),

        getCommonSpeed: () => this.ctrl.get("ptz_common_speed"),
        setCommonSpeed: (speed) => this.ctrl.set("ptz_common_speed", speed),

        getCommonTime: () => this.ctrl.get("ptz_common_time"),
        setCommonTime: (time) => this.ctrl.set("ptz_common_time", time),

        recallMode: () => this.ctrl.get("ptz_preset_mode"),
        setRecallMode: (mode) => this.ctrl.set("ptz_preset_mode", mode),

        recallSpeedMode: () => this.ctrl.get("ptz_speed_mode"),
        setRecallSpeedMode: (mode) => this.ctrl.set("ptz_speed_mode", mode),

        freezeDuringRecall: () => this.ctrl.get("freeze_during_preset"),
        setFreezeDuringRecall: (enable) => this.ctrl.set("freeze_during_preset", enable),

        ptzCommonSpeedUnit: () => this.ctrl.get("ptz_common_speed_unit"),
        setPtzCommonSpeedUnit: (unit) => this.ctrl.set("ptz_common_speed_unit", unit),
    }

    static ptrace = {
        delete: (index) => this.request(`/ctrl/ptrace?action=del&index=${index}`),
        setName: (index, name) => this.request(`/ctrl/ptrace?action=set_name&index=${index}&new_name=${encodeURIComponent(name)}`),

        recordStart: (index) => this.request(`/ctrl/ptrace?action=rec_start&index=${index}`),
        recordStop: () => this.request("/ctrl/ptrace?action=rec_stop"),

        playPrepare: (index) => this.request(`/ctrl/ptrace?action=play_prepare&index=${index}`),
        playStart: () => this.request("/ctrl/ptrace?action=play_start"),
        playStop: () => this.request("/ctrl/ptrace?action=play_stop"),

        query: () => this.request("/ctrl/ptrace?action=query"),
        getInfo: (index) => this.request(`/ctrl/ptrace?action=get_info&index=${index}`)
    }

    static user = {
        getUsers: ()=> this.request("/login/user"),
        add: (name, password, permission) => this.request(`/login/adduser?user=${encodeURIComponent(name)}&pswd=${password}&grp=${permission}`),
        delete: (name) => this.request(`/login/deluser?user=${encodeURIComponent(name)}`),
        changePassword: (name, oldPassword, newPassword) => this.request(`/login/pswd?user=${encodeURIComponent(name)}&old=${oldPassword}&new=${newPassword}`),
        logout: () => this.request("/login/quit")
    }

    static recording = {
        start: () => this.request('/ctrl/rec?action=start'),
        stop: () => this.request('/ctrl/rec?action=stop'),
        getStatus: () => this.request('/ctrl/rec?action=query'),
        getRemain: () => this.request('/ctrl/rec?action=remain'),
        getRepairStatus: () => this.request('/ctrl/rec?action=query_repairing'),
        
        tc: {
            query: () => this.request('/ctrl/tc?action=query'),
            reset: () => this.request('/ctrl/tc?action=reset'),
            setCurrent: () => this.request('/ctrl/tc?action=current_time'),
            setManual: (tc) => this.request(`/ctrl/tc?action=set&tc=${tc}`)
        },

        meta: {
            setRecordMeta: (enable) => this.ctrl.set("record_meta", enable),
            recordMeta: () => this.ctrl.get("record_meta"),
            
            setCameraId: (id) => this.ctrl.set("camera_id", id),
            cameraId: () => this.ctrl.get("camera_id"),

            setReelname: (reelname) => this.ctrl.set("reelname", reelname),
            reelname: () => this.ctrl.get("reelname")
        },

        resolution: () => this.ctrl.get("resolution"),
        setResolution: (resolution) => this.ctrl.set("resolution", resolution),

        projectFps: () => this.ctrl.get("project_fps"),
        setProjectFps: (fps) => this.ctrl.set("project_fps", fps),

        vfrCtrl: () => this.ctrl.get("vfr_ctrl"),
        setVfrCtrl: (enable) => this.ctrl.set("vfr_ctrl", enable),

        setMovVfr: (enable) => this.ctrl.set("movvfr", enable),
        movVfr: () => this.ctrl.get("movvfr"),

        fileFormat: () => this.ctrl.get("record_file_format"),
        setFileFormat: (format) => this.ctrl.set("record_file_format", format),

        fileRotation: () => this.ctrl.get("rotation"),
        setFileRotation: (rotation) => this.ctrl.set("rotation", rotation),

        splitDuration: () => this.ctrl.get("split_duration"),
        setSplitDuration: (duration) => this.ctrl.set("split_duration", duration),

        recFps: () => this.ctrl.get("rec_fps"),
        setRecFps: (fps) => this.ctrl.set("rec_fps", fps),

        rawOverHDMI: () => this.ctrl.get("raw_over_hdmi"),
        setRawOverHDMI: (enable) => this.ctrl.set("raw_over_hdmi", enable),

        recFrameIndicator: () => this.ctrl.get("rec_frame_indicator"),
        setRecFrameIndicator: (enable) => this.ctrl.set("rec_frame_indicator", enable),

        preroll: {
            setPreroll: (enable) => this.ctrl.set("preroll", enable),
            preroll: () => this.ctrl.get("preroll"),

            prerollDuration: () => this.ctrl.get("preroll_duration"),
            setPrerollDuration: (duration) => this.ctrl.set("preroll_duration", duration)
        },
    }

    static photo = {
        capture: () => this.request('/ctrl/still?action=cap')
    }

    static playback = {
        query: () => this.request('/ctrl/pb?action=query')
    }

    static sync = {
        pixlelink : {

        },
        ezlink : {
            mode: () => this.ctrl.get("ezlink_mode"),
            setMode: (mode) => this.ctrl.set("ezlink_mode", mode),

            trigger: () => this.ctrl.get("ezlink_trigger"),
            setTrigger: (trigger) => this.ctrl.set("ezlink_trigger", trigger)
        },
        genlock: {
            status: () => this.ctrl.get("genlock"),
            setStatus: (status) => this.ctrl.set("genlock", status),

            coarse: () => this.ctrl.get("genlock_coarse"),
            setCoarse: (coarse) => this.ctrl.set("genlock_coarse", coarse),

            fine: () => this.ctrl.get("genlock_fine"),
            setFine: (fine) => this.ctrl.set("genlock_fine", fine)
        }
    }

    static video = {
        setEncoder: (encoder) => this.ctrl.set("video_encoder", encoder),
        encoder: () => this.ctrl.get("video_encoder"),

        setBitrateLevel: (bitrate) => this.ctrl.set("bitrate_level", bitrate),
        bitrateLevel: () => this.ctrl.get("bitrate_level"),

        setComposeMode: (mode) => this.ctrl.set("compose_mode", mode),
        composeMode: () => this.ctrl.get("compose_mode"),

        setPhotoQuality: (q) => this.ctrl.set("photo_q", q),
        photoQuality: () => this.ctrl.get("photo_q"),

        setEisOnOff: (enable) => this.ctrl.set("eis_on_off", enable),
        eisOnOff: () => this.ctrl.get("eis_on_off"),

        setVideoRotation: (rotation) => this.ctrl.set("vid_rot", rotation),
        videoRotation: () => this.ctrl.get("vid_rot"),

        recordMode: () => this.ctrl.get("record_mode"),
        setRecordMode: (mode) => this.ctrl.set("record_mode", mode),

        videoTimelapseInterval: () => this.ctrl.get("video_tl_interval"),
        setVideoTimelapseInterval: (interval) => this.ctrl.set("video_tl_interval", interval),

        lowJello: () => this.ctrl.get("low_jello"),
        setLowJello: (enable) => this.ctrl.set("low_jello", enable),
    }

    static audio = {
        setEncoder: (encoder)=> this.ctrl.set("primary_audio", encoder),
        encoder: ()=> this.ctrl.get("primary_audio"),

        setInChannel: (channel)=> this.ctrl.set("audio_channel", channel),
        inChannel:()=> this.ctrl.get("audio_channel"),

        setPhantomPower: (power)=> this.ctrl.set("audio_phantom_power", power),
        phantomPower: ()=> this.ctrl.get("audio_phantom_power"),

        setLevelDisplay: (display)=> this.ctrl.set("audio_level_display", display),
        levelDisplay: ()=> this.ctrl.get("audio_level_display"),

        setInGainType: (type)=> this.ctrl.set("ain_gain_type", type),
        inGainType: ()=> this.ctrl.get("ain_gain_type"),

        setInLevel: (level)=> this.ctrl.set("audio_input_level", level),
        inLevel: ()=> this.ctrl.get("audio_input_level"),

        setInGain: (gain)=> this.ctrl.set("audio_input_gain", gain),
        inGain: ()=> this.ctrl.get("audio_input_gain"),

        setInLeftGain: (gain)=> this.ctrl.set("audio_in_l_gain", gain),
        inLeftGain: ()=> this.ctrl.get("audio_in_l_gain"),

        setInRightGain: (gain)=> this.ctrl.set("audio_in_r_gain", gain),
        inRightGain: ()=> this.ctrl.get("audio_in_r_gain"),

        setOutputGain: (gain)=> this.ctrl.set("audio_output_gain", gain),
        outputGain: ()=> this.ctrl.get("audio_output_gain"),

        setNoiseReduction: (enable)=> this.ctrl.set("audio_noise_reduction", enable),
        noiseReduction: ()=> this.ctrl.get("audio_noise_reduction")
    }

    static profile = {
        export: () => this.request('/prf/export')
    }

    static security = {
        https: () => this.ctrl.get("https_on"),
        setHttps: (enable) => this.ctrl.set("https_on", enable),

        auth: () => this.ctrl.get("https_auth"),
        setAuth: (enable) => this.ctrl.set("https_auth", enable),

        certificate : {
            source: () => this.ctrl.get("https_cert_source"),
            setSource: (source) => this.ctrl.set("https_cert_source", source),

            generate: () => this.request("/cert/gen"),

            certInfo: () => this.request("/cert/info"),

            deleteCert: () => this.request("/cert/delete"),
        }
    }

    static streaming = {
        rtmp: {
            query: (index) => this.request(`/ctrl/rtmp?action=query&index=${index}`),
            start: (url, key) => {
                let params = 'action=start';
                if (url && url.length > 0) params += `&url=${encodeURIComponent(url)}`;
                if (key && key.length > 0) params += `&key=${encodeURIComponent(key)}`;
                return this.request(`/ctrl/rtmp?${params}`);
            },
            stop: (index) => this.request(`/ctrl/rtmp?action=stop&index=${index}`)
        },

        srt: {
            query: () => this.request('/ctrl/srt?action=query'),
            start: (url) => this.request(`/ctrl/srt?action=start&url=${encodeURIComponent(url)}`),
            stop: () => this.request('/ctrl/srt?action=stop'),
            set: (mode, passphrase, pbkeyLen, latency, ttl) => {
                const params = new URLSearchParams();
                params.append('action', 'set');
                if (mode) params.append('mode', mode);  
                params.append('passphrase', passphrase);
                if (pbkeyLen) params.append('pbkeyLen', pbkeyLen);
                if (latency) params.append('latency', latency);
                if (ttl) params.append('ttl', ttl);

                return this.request(`/ctrl/srt?${params.toString()}`);
            }
        },
        
        // NDI控制
        ndi: {
            query: () => this.request('/ctrl/ndi?action=query'),
            set: (machine, stream, group, ttl,
                    multcast, multcast_net, multicast_prefix,
                    discover1, discover2,
                    bridge_name, bridge_encry, bridge_server, bridge_port) => {

                let params = "action=set";
                if (machine && machine.length > 0) params += `&machine=${encodeURIComponent(machine)}`;
                if (stream && stream.length > 0) params += `&stream=${encodeURIComponent(stream)}`;
                params += `&group=${encodeURIComponent(group)}`;
                if (ttl) params += `&ttl=${ttl}`;
                if (multcast) params += `&multicastEnable=${multcast}`;
                params += `&discoveryServer1=${encodeURIComponent(discover1)}`;
                params += `&discoveryServer2=${encodeURIComponent(discover2)}`;
                if (multcast_net && multcast_net.length > 0) params += `&multicastNetmask=${encodeURIComponent(multcast_net)}`;
                if (multicast_prefix && multicast_prefix.length > 0) params += `&multicastPrefix=${encodeURIComponent(multicast_prefix)}`;
                params += `&bridgeName=${encodeURIComponent(bridge_name)}`;
                params += `&bridgeEncryptionKey=${encodeURIComponent(bridge_encry)}`;
                params += `&bridgeServer=${encodeURIComponent(bridge_server)}`;
                if (bridge_port) params += `&bridgePort=${bridge_port}`;

                return this.request(`/ctrl/ndi?${params}`);
            }
        },

        rtsp : {
            query: () => this.request('/ctrl/rtsp?action=query'),
            setAuth: (auth) => this.request(`/ctrl/rtsp?action=set&authentication=${auth}`)
        },

        query: (index) => this.request(`/ctrl/stream_setting?action=query&index=stream${index}`),
        setBitrate: (index, bitrate) => this.request(`/ctrl/stream_setting?index=stream${index}&bitrate=${bitrate}`),
        performance: (index) => this.request(`/ctrl/stream_setting?action=stream_performance&index=stream${index}`),

        resolution: () => this.ctrl.get("stream_resolution"),
        setResolution: (resolution) => this.ctrl.set("stream_resolution", resolution),

        fps: () => this.ctrl.get("stream_fps"),
        setFps: (fps) => this.ctrl.set("stream_fps", fps),

        encoder: () => this.ctrl.get("stream_video_encoder"),
        setEncoder: (encoder) => this.ctrl.set("stream_video_encoder", encoder),

        streamParamSave: () => this.ctrl.get("stream_param_save"),
        setStreamParamSave: (enable) => this.ctrl.set("stream_param_save", enable)
    }

    static assitool = {
        setDisplay: (enable) => this.ctrl.set("assitool_display", enable),
        display: () => this.ctrl.get("assitool_display"),

        setScope: (scope) => this.ctrl.set("assitool_scope", scope),
        scope: () => this.ctrl.get("assitool_scope"),

        setPeak: (enable) => this.ctrl.set("assitool_peak_onoff", enable),
        peak: () => this.ctrl.get("assitool_peak_onoff"),

        setPeakColor: (color) => this.ctrl.set("assitool_peak_color", color),
        peakColor: () => this.ctrl.get("assitool_peak_color"),

        setFrameLine: (enable) => this.ctrl.set("assitool_frame_line", enable),
        frameLine: () => this.ctrl.get("assitool_frame_line"),

        setFrameLineColor: (color) => this.ctrl.set("assitool_frame_line_color", color),
        frameLineColor: () => this.ctrl.get("assitool_frame_line_color"),

        setFrameLineOpa: (opacity) => this.ctrl.set("assitool_frame_line_opacity", opacity),
        frameLineOpa: () => this.ctrl.get("assitool_frame_line_opacity"),

        setCenterMark: (enable) => this.ctrl.set("assitool_center_mark", enable),
        centerMark: () => this.ctrl.get("assitool_center_mark"),

        setCenterMarkColor: (color) => this.ctrl.set("assitool_center_mark_color", color),
        centerMarkColor: () => this.ctrl.get("assitool_center_mark_color"),

        setExposure: (exposure) => this.ctrl.set("assitool_exposure", exposure),
        exposure: () => this.ctrl.get("assitool_exposure"),

        setZera1: (zera) => this.ctrl.set("assitool_zera_th1", zera),
        zera1: () => this.ctrl.get("assitool_zera_th1"),

        setZera2: (zera) => this.ctrl.set("assitool_zera_th2", zera),
        zera2: () => this.ctrl.get("assitool_zera_th2"),

        setGridLine: (enable) => this.ctrl.set("assitool_grid_line", enable),
        gridLine: () => this.ctrl.get("assitool_grid_line"),

        setSafaArea: (enable) => this.ctrl.set("assitool_safe_area", enable),
        safaArea: () => this.ctrl.get("assitool_safe_area"),

        setHDMIFocosArea: (enable) => this.ctrl.set("hdmi_focus_area", enable),
        HDMIFocosArea: () => this.ctrl.get("hdmi_focus_area")
    }

    static timecode = {
        setSrc: (enable) => this.ctrl.set("tc_source", enable),
        src: () => this.ctrl.get("tc_source"),

        setCountUp: (type) => this.ctrl.set("tc_count_up", type),
        countUp: () => this.ctrl.get("tc_count_up"),

        setHDMIDispaly: (enable) => this.ctrl.set("tc_hdmi_dispaly", enable),
        HDMIDispaly: () => this.ctrl.get("tc_hdmi_dispaly"),

        etDropFrame: (df) => this.ctrl.set("tc_drop_frame", df),
        dropFrame: () => this.ctrl.get("tc_drop_frame"),
    }

    static multicam = {
        setUnionAE: (ae) => this.ctrl.set("union_ae", ae),
        unionAE: () => this.ctrl.get("union_ae"),

        setUnionAWB: (awb) => this.ctrl.set("union_awb", awb),
        unionAWB: () => this.ctrl.get("union_awb"),

        setEzLinkMode: (mode) => this.ctrl.set("ezlink_mode", mode),
        ezLinkMode: () => this.ctrl.get("ezlink_mode"),

        setEzLinkTrigger: (mode) => this.ctrl.set("ezlink_trigger", mode),
        ezLinkTrigger: () => this.ctrl.get("ezlink_trigger"),
    }

    static exposure = {
        setEv: (ev) => this.ctrl.set("ev", ev),
        ev: () => this.ctrl.get("ev"),

        setAntiFlicker: (flicker) => this.ctrl.set("flicker", flicker),
        antiFlicker: () => this.ctrl.get("flicker"),

        setMeterMode: (mode) => this.ctrl.set("meter_mode", mode),
        meterMode: () => this.ctrl.get("meter_mode"),

        setIris: (iris) => this.ctrl.set("iris", iris),
        iris: () => this.ctrl.get("iris"),

        setIso: (iso) => this.ctrl.set("iso", iso),
        iso: () => this.ctrl.get("iso"),

        setMinIso: (min) => this.ctrl.set("min_iso", min),
        minIso: () => this.ctrl.get("min_iso"),

        setMaxIso: (max) => this.ctrl.set("max_iso", max),
        maxIso: () => this.ctrl.get("max_iso"),

        setIsoControl: (enable) => this.ctrl.set("iso_ctrl", enable),
        isoControl: () => this.ctrl.get("iso_ctrl"),

        setShutterAngleControl: (enable) => this.ctrl.set("shutter_angle_ctrl", enable),
        shutterAngleControl: () => this.ctrl.get("shutter_angle_ctrl"),

        setShutterOperation: (operation) => this.ctrl.set("sht_operation", operation),
        shutterOperation: () => this.ctrl.get("sht_operation"),

        setElecND: (enable) => this.ctrl.set("eND", enable),
        elecND: () => this.ctrl.get("eND"),

        setAeSpeed: (speed) => this.ctrl.set("ae_speed", speed),
        aeSpeed: () => this.ctrl.get("ae_speed"),

        setLockAeInRec: (enable) => this.ctrl.set("lock_ae_in_rec", enable),
        lockAeInRec: () => this.ctrl.get("lock_ae_in_rec"),

        setBacklightComp: (enable) => this.ctrl.set("bl_comp", enable),
        backlightComp: () => this.ctrl.get("bl_comp")
    }

    static whiteBalance = {
        setMode: (mode) => this.ctrl.set("wb", mode),
        mode: () => this.ctrl.get("wb"),

        setManualKelvin: (kelvin) => this.ctrl.set("mwb", kelvin),
        manualKelvin: () => this.ctrl.get("mwb"),

        setManualTint: (tint) => this.ctrl.set("tint", tint),
        manualTint: () => this.ctrl.get("tint"),

        setManualR: (r) => this.ctrl.set("mwb_r", r),
        manualR: () => this.ctrl.get("mwb_r"),

        setManualG: (g) => this.ctrl.set("mwb_g", g),
        manualG: () => this.ctrl.get("mwb_g"),

        setManualB: (b) => this.ctrl.set("mwb_b", b),
        manualB: () => this.ctrl.get("mwb_b"),

        setAwbPriority: (priority) => this.ctrl.set("wb_priority", priority),
        awbPriority: () => this.ctrl.get("wb_priority"),

        setLockAwbInRec: (enable) => this.ctrl.set("lock_awb_in_rec", enable),
        lockAwbInRec: () => this.ctrl.get("lock_awb_in_rec"),

        onePushWb: () => this.request("/ctrl/wb?action=one_push"),
    }

    static image = {
        basic : {
            setImageProfile: (profile) => this.ctrl.set("lut", profile),
            imageProfile: () => this.ctrl.get("lut"),

            setNoiseReduction: (enable) => this.ctrl.set("noise_reduction", enable),
            noiseReduction: () => this.ctrl.get("noise_reduction"),

            setLumaLevel: (level) => this.ctrl.set("luma_level", level),
            lumaLevel: () => this.ctrl.get("luma_level"),

            setBrightness: (brightness) => this.ctrl.set("brightness", brightness),
            brightness: () => this.ctrl.get("brightness"),

            setContrast: (contrast) => this.ctrl.set("contrast", contrast),
            contrast: () => this.ctrl.get("contrast"),

            setSharpness: (sharpness) => this.ctrl.set("sharpness", sharpness),
            sharpness: () => this.ctrl.get("sharpness"),

            setSaturation: (saturation) => this.ctrl.set("saturation", saturation),
            saturation: () => this.ctrl.get("saturation"),

            setHue: (hue) => this.ctrl.set("hue", hue),
            hue: () => this.ctrl.get("hue")
        },
        matrix : {
            query: () => this.request('/ctrl/matrix'),
            enable: (value) => this.request(`/ctrl/matrix?enable=${value}`),
            set: (index, value) => this.request(`/ctrl/matrix?index=${index}&value=${value}`)
        },

        colorCorrection: {
            query: () => this.request('/ctrl/cc'),
            enable: (value) => this.request(`/ctrl/cc?enable=${value}`),
            setHue: (index, value) => this.request(`/ctrl/cc?action=set_hue&index=${index}&value=${value}`),
            setSat: (index, value) => this.request(`/ctrl/cc?action=set_sat&index=${index}&value=${value}`)
        },

        gamma: {
            get: (which) => this.request(`/ctrl/gamma?action=get&option=${which}`),
            base : {
                query: () => this.request('/ctrl/gamma?action=get&option=gamma'),
                setPower: (base, power) => this.request(`/ctrl/gamma?action=set&option=gamma&base=${base}&power=${power}`)
            },
            blackLevel: {
                query: () => this.request('/ctrl/gamma?action=get&option=black_level'),
                enable: (enable, level) => {
                    const params = new URLSearchParams();
                    params.append('action', 'set');
                    params.append('option', 'black_level');
                    params.append('enable', enable);
                    params.append('level', level);
                    return this.request(`/ctrl/gamma?${params.toString()}`);
                }
            },
            blackGamma: {
                query: () => this.request('/ctrl/gamma?action=get&option=black_gamma'),
                enable: (enable, range, level) => {
                    const params = new URLSearchParams();
                    params.append('action', 'set');
                    params.append('option', 'black_gamma');
                    params.append('enable', enable);
                    params.append('range', range);
                    params.append('level', level);
                    return this.request(`/ctrl/gamma?${params.toString()}`);
                }
            },
            knee: {
                query: () => this.request('/ctrl/gamma?action=get&option=knee'),
                enable: (enable, point, slope) => {
                    const params = new URLSearchParams();
                    params.append('action', 'set');
                    params.append('option', 'knee');
                    params.append('enable', enable);
                    params.append('point', point);
                    params.append('slope', slope);
                    return this.request(`/ctrl/gamma?${params.toString()}`);
                }
            }
        }
    }

    static framing = {
        enableDetect: (enable) => this.request(`/ctrl/framing/detect?enable=${enable}`),
        enableTrace: (enable) => this.request(`/ctrl/framing/trace?enable=${enable}`),
        setTraceTarget: (id) => this.request(`/ctrl/framing/trace_target?id=${id}`),

        setStartPos: (index) => this.request(`/ctrl/framing/start_position?index=${index}`),

        setFovType: (type) => this.request(`/ctrl/framing/fov?type=${type}`),
        setFovBodySize: (size) => this.request(`/ctrl/framing/fov?body_size=${size}`),
        setFovFaceHeight: (height) => this.request(`/ctrl/framing/fov?face_height=${height}`),
        setFovPlacement: (placement) => this.request(`/ctrl/framing/fov?placement=${placement}`),
        debugFov: () => this.request('/ctrl/framing/debug_fov'),

        setAutoStart: (enable) => this.request(`/ctrl/framing/auto_start?enable=${enable}`),
        restart: () => this.request('/ctrl/framing/restart'),

        setAutoTarget: (enable) => this.request(`/ctrl/framing/auto_target?enable=${enable}`),
    
        setTargetLostAction: (action) => this.request(`/ctrl/framing/target_lost?action=${action}`),
        setTargetLostTimeout: (timeout) => this.request(`/ctrl/framing/target_lost?timeout_sec=${timeout}`),
        getTargetLostTimeout: () => this.request('/ctrl/framing/target_lost?action=query'),
    
        setDetectRestartTime: (time) => this.request(`/ctrl/framing/detect_restart?time=${time}`),
        getDetectRestartTime: () => this.request('/ctrl/framing/detect_restart?action=query'),

        setLostActionWait: (wait) => this.request(`/ctrl/framing/manual_target?wait_time=${wait}`),
        getLostActionWait: () => this.request('/ctrl/framing/manual_target?action=query'),

        status: () => this.request('/ctrl/framing/status'),
        allStatus: () => this.request('/ctrl/framing/all_status'),
        info: () => this.request('/ctrl/framing/info'),
        
        tally: (enable) => this.request(`/ctrl/framing/tally?enable=${enable}`),

        maskEnable: (enable) => this.request(`/ctrl/framing/mask_area?enable=${enable}`),
        maskArea: () => this.request('/ctrl/framing/mask_area'),
        setMaskArea: (left, top, right, bottom) => this.request(`/ctrl/framing/mask_area?left=${left}&top=${top}&right=${right}&bottom=${bottom}`),

        setTrackActiveFace: (enable) => this.request(`/ctrl/framing/track_active_face?enable=${enable}`),
        getTrackActiveFace: () => this.request(`/ctrl/framing/track_active_face`),

        setTargetNum: (num) => this.request(`/ctrl/framing/target_num?num=${num}`),
        getTargetNum: () => this.request(`/ctrl/framing/target_num`),

        setLockTilt: (enable) => this.request(`/ctrl/framing/tilt_lock?enable=${enable}`),
        getLockTilt: () => this.request(`/ctrl/framing/tilt_lock`),
    }

    static ezframing = {
        center: (x, y, speed) => this.request(`/ctrl/ezframing/center?x=${x}&y=${y}&speed=${speed}`),
        roi: (x, y, speed, ratio) => this.request(`/ctrl/ezframing/roi?x=${x}&y=${y}&speed=${speed}&ratio=${ratio}`)
    }

    static facedb = {
        query: () => this.request('/ctrl/facedb/query'),
        prepare: (uid) => this.request(`/ctrl/facedb/prepare?face_id=${uid}`),
        add: (uid, name, priority, activate) => this.request(`/ctrl/facedb/add?which=${uid}&name=${name}&priority=${priority}&activate=${activate}`),
        update: (uid, name, priority, activate) => this.request(`/ctrl/facedb/update?uid=${uid}&name=${name}&priority=${priority}&activate=${activate}`),
        delete: (uid) => this.request(`/ctrl/facedb/delete?uid=${uid}`),
        enable: (enable) => this.request(`/ctrl/facedb/set?enable=${enable}`),
        status: () => this.request('/ctrl/facedb/status')
    }

    static upgrade = {
        check: () => this.request("/ctrl/upgrade?action=fw_check"),
        run: () => this.request("/ctrl/upgrade?action=run"),
        uiCheck: () => this.request("/ctrl/upgrade?action=ui_check")
    }

    static files = {
        listFolders: () => this.request("/DCIM/"),
        listFiles: (folder) => this.request(`/DCIM/${folder}/`)
    }

    static system = {
        clearSetting: () => this.request('/ctrl/set?action=clear'),
        temperature: () => this.request('/ctrl/temperature'),

        tallyBrightness: () => this.ctrl.get("tally_on"),
        setTallyBrightness: (brightness) => this.ctrl.set("tally_on", brightness),

        colorBar: () => this.ctrl.get("color_bar_enable"),
        setColorBar: (enable) => this.ctrl.set("color_bar_enable", enable),

        led: () => this.ctrl.get("led"),
        setLed: (enable) => this.ctrl.set("led", enable),

        desqueeze: () => this.ctrl.get("desqueeze"),
        setDesqueeze: (enable) => this.ctrl.set("desqueeze", enable),

        freed: {
            query: () => this.request('/ctrl/freed'),
            setCameraID: (id) => this.request(`/ctrl/freed?camera_id=${id}`),
            setConfig: ({ ip, port }) => this.request(`/ctrl/freed?ip=${encodeURIComponent(ip)}&port=${port}`),
            enable: (value) => this.request(`/ctrl/freed?enable=${value}`)
        },

        visca: {
            setId: (id) => this.ctrl.set("visca_id", id),
            setBaudrate: (baudrate) => this.ctrl.set("visca_baudrate", baudrate),
            enable: (value) => this.ctrl.set("visca_enable", value)
        },

        time: {
            get: () => this.request('/datetime/get'),
            set: (date, time) => this.request(`/datetime?date=${date}&time=${time}`),
            setTimezone: (timezone) => this.request(`/datetime?timezone=${timezone}`)
        },
    
        snmp: {
            enable: (value) => this.request(`/ctrl/snmp?enable=${value}`),
            setAuthEnc: (user, auth, enc, password) => {
                return this.request(`/ctrl/snmp?user=${encodeURIComponent(user)}&auth=${auth}&enc=${enc}&pass=${encodeURIComponent(password)}`);
            },
            setSystemName: (name) => this.request(`/ctrl/snmp?system=${encodeURIComponent(name)}`),
            setLocation: (location) => this.request(`/ctrl/snmp?location=${encodeURIComponent(location)}`),
            setContact: (contact) => this.request(`/ctrl/snmp?contact=${encodeURIComponent(contact)}`),
            query: () => this.request('/ctrl/snmp')
        },

        sntp: {
            start: (addr, port, interval) => this.request(`/ctrl/sntp?action=start&ip_addr=${encodeURIComponent(addr)}&port=${port}&interval=${interval}`),
            stop: () => this.request('/ctrl/sntp?action=stop'),
            getStatus: () => this.request('/ctrl/sntp?action=get'),
            setInterval: (interval) => this.request(`/ctrl/sntp?action=set_interval&interval=${interval}`)
        },

        card: {
            isPresent: () => this.request('/ctrl/card?action=present'),
            queryFormat: () => this.request('/ctrl/card?action=query_format'),
            format: () => this.request('/ctrl/card?action=format')
        },

        power: {
            shutdown: () => this.request('/ctrl/shutdown', {timeout: 1000}),
            reboot: () => this.request('/ctrl/reboot', {timeout: 1000}),
            standby: () => this.request('/ctrl/mode?action=to_standby'),
            wakeup: () => this.request('/ctrl/mode?action=exit_standby'),

            autoOff: () => this.ctrl.get("auto_off"),
            setAutoOff: (time) => this.ctrl.set("auto_off", time),

            autoStandby: () => this.ctrl.get("auto_standby"),
            setAutoStandby: (time) => this.ctrl.set("auto_standby", time),
        }
    }

    static connection = {
        usb : {
            role: () => this.ctrl.get("usb_device_role"),
            setRole: (role) => this.ctrl.set("usb_device_role", role),

            query: () => this.request('/ctrl/usb?action=query')
        },

        ir : {
            enable: (enable) => this.ctrl.set("ir", enable),

            id: () => this.ctrl.get("ir_id"),
            setId: (id) => this.ctrl.set("ir_id", id)
        },

        sdi : {
            output: () => this.ctrl.get("sdi"),
            setOutput: (enable) => this.ctrl.set("sdi", enable),

            mode3G: () => this.ctrl.get("3g_sdi_mode"),
            setMode3G: (mode) => this.ctrl.set("3g_sdi_mode", mode)
        },

        hdmi : {
            format: () => this.ctrl.get("hdmi_format"),
            setFormat: (format) => this.ctrl.set("hdmi_format", format),

            osd: () => this.ctrl.get("hdmi_osd"),
            setOsd: (enable) => this.ctrl.set("hdmi_osd", enable),

            layout: () => this.ctrl.get("osd_layout"),
            setLayout: (layout) => this.ctrl.set("osd_layout", layout)
        }
    }

    static network = {
        ethernet: {
            info: () => this.request('/ctrl/network?action=info'),
            mode: () => this.ctrl.get("eth_mode"),
            setStatic: (ip, netmask, gateway, dns) => {
                const params = new URLSearchParams();
                params.append('action', 'set');
                params.append('mode', 'static');
                if (ip) params.append('ipaddr', ip);
                if (netmask) params.append('netmask', netmask);
                if (gateway) params.append('gateway', gateway);
                if (dns) params.append('dns', dns);
                return this.request(`/ctrl/network?${params.toString()}`);
            }
        },

        wifi: {
            query: () => this.request('/ctrl/wifi_ctrl?action=query'),

            setWifi: (enable) => this.ctrl.set("wifi", enable),
            wifi: () => this.ctrl.get("wifi"),

            setWifiChannel: (enable) => this.ctrl.set("wifi_channel", enable),
            wifiChannel: () => this.ctrl.get("wifi_channel"),
        }
    }
}
